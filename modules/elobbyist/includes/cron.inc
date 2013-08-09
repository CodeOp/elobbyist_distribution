<?php

/*
  * @file
  * Encapsulates the main cron job task for elobbyist 
 * 
  * See the Run() method
  */



class ElobbyistCronTask {
  
  protected $Mailings = null;
  
   /*
    * Processes active mailings that have already started
    */
  public function Run() {
    try {
      $Sent = 0;
      
      $MaxSend = variable_get('elobbyist_max_emails_per_batch', ELOBBYIST_DEFAULT_MAX_EMAILS_PER_BATCH);
            
      //load mail library
      require_once dirname(__FILE__) . '/Elobbyist_Mail.inc';
      
      $this->LoadMailings();
      
      foreach($this->Mailings as $mailing) {
        $mail_obj = new ElobbyistCronTaskMailing($mailing, $MaxSend);
        $mail_obj->Process($Sent);
        unset($mail_obj);
        if ($Sent >= $MaxSend && $MaxSend > 0)
          break;
      }
    }
    catch(Exception $ex) {
      watchdog('elobbyist cron', $ex->getMessage() . ' Trace: ' . $ex->getTraceAsString(), array(), WATCHDOG_ERROR);
    }
  }
  
  protected function LoadMailings() {
    $query = db_select('elobbyist_mailing', 'm');
    $query->join('elobbyist_template', 't', 'm.tid = t.tid');    
    
    $this->Mailings = $query->condition('m.status', ELOBBYIST_MAILING_STATUS_ACTIVE)
          ->condition('start', REQUEST_TIME, '<=')
          ->fields ('m', array (
                  'mid',
                  'schedule',
                  'end',
                  'addtogrpid',
                )
            )
          ->fields ('t', array (
                  'tid',
                  'subject',
                  'body',
                )
            )
          ->execute();
  }
}

class ElobbyistCronTaskMailing {
  
  protected $Mailing = null;
  protected $FromGroups = array();
  protected $ToGroups = array();
  protected $FromIds = array();
  protected $CollectedSendersIds = array();
  protected $ToIds = array();
  protected $From = null;
  protected $CollectedSenders = null;
  protected $To = null;
  
  protected $NotificationSenderReady = FALSE;
  protected $NotificationSenderEmail = null;
  protected $NotificationSenderSmtpHost = null;
  protected $NotificationSenderSmtpPort = null;
  protected $NotificationSenderPass = null;
  protected $NotificationSubject = null;
  protected $MaxBounces = 0;
  
  protected $Sent = 0;
  protected $MaxSend = 0;
  
  protected $ProcessFrom = TRUE; //used by NextSender()
  
  protected $Activities = array();

  public function __construct($row, $max_send) {
    $this->Mailing = $row;
    $this->MaxSend = $max_send;
  }

   /*
    * Process emails for each mailing
    */
  public function Process(&$total_sent) {
    if (!$this->LoadContacts()) {
      return; //exit (skip this mailing) if there are no recipients/senders
    }
    
    $this->PrepareNotifications();
    
    //load already processed activities
    $this->LoadActivities();

    $Recipient = null;
    $bMaxSendTest = FALSE;
    $Sender = $this->FirstSender();
    do {
      $bNotify = FALSE;
      reset($this->To);
      $Recipient = current($this->To);
      $Body = NULL;
      $Pass = $this->Decrypt($Sender->pass);
      $RecipientEmailList = array();
      $RecipientNameList = array();
      
      do {
        //skip processed mails
        if (!$this->HasActivity($Sender->cid, $Recipient->cid)) {
          //create the activity and send email
          if ($this->ProcessEmail($Sender, $Recipient, $Body, $Pass)) {
            //collect recipient emails for notifications
            if ($Sender->notify && $this->NotificationSenderReady) {
              $bNotify = TRUE;
              $RecipientEmailList[] = $Recipient->email;
              $RecipientNameList[]  = $Recipient->name;
            }
            $total_sent++;
            $bMaxSendTest = ($total_sent >= $this->MaxSend && $this->MaxSend > 0);
          }
          elseif ($Sender->bounce == ELOBBYIST_CONTACT_EMAIL_BOUNCING_IGNORED) {
            break;
          }
        }
      } while(!$bMaxSendTest && $Recipient = next($this->To));
      if ($bNotify) {
        $this->NotifySender($Sender, $Body, $RecipientEmailList, $RecipientNameList);
      }
    } while (!$bMaxSendTest && $Sender = $this->NextSender());

    if (!$bMaxSendTest) {
      $this->Close();
    }
  }
  
  /*
   * Create the activity and send the email
   */
  protected function ProcessEmail($Sender, $Recipient, &$Body, &$Pass) {   
    //send email
    $oMailResponse = NULL;
    $oEmail = NULL;
    $oMailObj = NULL;
    try
    {
      //load sender email provider
      $eps = entity_load('elobbyist_email_provider', array($Sender->providerid));
      if (!isset($eps) || count($eps) == 0)
        return FALSE;
      reset($eps);
      $ep = current($eps);
            
      $oEmail = new Elobbyist_MailInfo();
      $oEmail->From = $Sender->email;
      $oEmail->To = $Recipient->email;
      $oEmail->Subject = elobbyist_template_to_email_subject(
        $this->Mailing->subject, $Sender->name, $Recipient->name );
      $oEmail->Body = elobbyist_template_to_email_body($this->Mailing->body, $Sender->name,
              $Recipient->name);
      //update body for notifications
      $Body = $oEmail->Body;
      $oEmail->Host = $ep->smtphost;
      $oEmail->Port = $ep->smtpport;
      $oEmail->Password = $Pass;
      
      $oMailObj = $this->GetMailObject();
      
      $oMailResponse = $oMailObj->SendMail($oEmail);

      if (!$oMailResponse->Success) {
        if ($oMailResponse->Bounced) {
          $this->SetBouncing($Sender);
        }
        $this->LogMailError(NULL, 'recipient', $oMailResponse, $oEmail);
        return FALSE;
      }
      else {
        if ($Sender->bounce != ELOBBYIST_CONTACT_EMAIL_VERIFIED) {
          $Sender->bounce = ELOBBYIST_CONTACT_EMAIL_VERIFIED;
          $Sender = entity_get_controller('elobbyist_contact')->save($Sender);
        }
        return $this->CreateActivity($Sender, $Recipient);
      }
    }
    catch(Exception $ex) {
      $this->LogMailError($ex, 'recipient', $oMailResponse, $oEmail);
    }
    
    return FALSE;
  }

  /*
   * Gets the next sender or FALSE if there aren't any - from either the From array or the CollectedSenders one
   * If the next sender is from the From array and is confined to a mailing - it is skipped
   */
  protected function NextSender() {    
    if ($this->ProcessFrom) {
      $Sender = next($this->From);
      if (!$Sender) {
        $this->ProcessFrom = FALSE;
        if (!isset($this->CollectedSenders) || count($this->CollectedSenders) == 0) {
          return FALSE;
        }
        return $this->NextSender();
      }
      if ($this->SkipSender($Sender)) {
        return $this->NextSender();
      }
      return $Sender;
    }
    else {
      $Sender = next($this->CollectedSenders);
      if (!$Sender)
        return FALSE;
      if ($this->SkipSender($Sender)) {
        return $this->NextSender();
      }
      return $Sender;
    }
  }
  
  /*
   * Gets the first sender or FALSE if there aren't any - from either the From array or the CollectedSenders one
   */
  protected function FirstSender() {   
    if (!isset($this->From) || count($this->From) == 0) {
      $this->ProcessFrom = FALSE;
    }
      
    if ($this->ProcessFrom) {
      $Sender = current($this->From);
      if ($this->SkipSender($Sender)) {
        return $this->NextSender();
      }
      return $Sender;
    }
    else {
      if (!isset($this->CollectedSenders) || count($this->CollectedSenders) == 0) {
        return FALSE;
      }
      $Sender = current($this->CollectedSenders);
      if ($this->SkipSender($Sender)) {
        return $this->NextSender();
      }
      return $Sender;
    }
  }
  
  protected function SetBouncing($Sender) {
    $bSave = FALSE;
    $Sender->bounces++;
    if ($Sender->bounces >= $this->MaxBounces && $this->MaxBounces > 0) {
      if ($Sender->bounce != ELOBBYIST_CONTACT_EMAIL_BOUNCING_IGNORED) {
        $Sender->bounce = ELOBBYIST_CONTACT_EMAIL_BOUNCING_IGNORED;
        $bSave = TRUE;
      }
    }
    elseif ($Sender->bounce != ELOBBYIST_CONTACT_EMAIL_BOUNCING) {
      $Sender->bounce = ELOBBYIST_CONTACT_EMAIL_BOUNCING;
      $bSave = TRUE;
    }
    elseif ($Sender->bounces < $this->MaxBounces) {
      $bSave = TRUE; //just save the count
    }
    if ($bSave) {
      $Sender = entity_get_controller('elobbyist_contact')->save($Sender);
    }   
 }
  
  protected function LoadContacts() {
    //prepare for contact loading
    $this->ProcessRelations();
    //exit (skip this mailing) if there are no recipients
    if (count($this->ToIds) == 0) {
      return FALSE;
    }

    $Collected = count($this->CollectedSendersIds);

    if (count($this->FromIds) > 0) {
      $this->From = entity_load('elobbyist_contact', $this->FromIds);
      reset($this->From);
    }
    elseif ($Collected == 0) {
      //exit (skip this mailing) if there are no senders
      return FALSE;
    }

    if ($Collected > 0) {
      $this->CollectedSenders = entity_load('elobbyist_contact', $this->CollectedSendersIds);
      reset($this->CollectedSenders);
    }

    $this->To = entity_load('elobbyist_contact', $this->ToIds);
    reset($this->To);

    //get max bouncing variable value
    $this->MaxBounces = variable_get('elobbyist_max_bounces', ELOBBYIST_DEFAULT_MAX_BOUNCES);

    return TRUE;
  }
  
  protected function CreateActivity($Sender, $Recipient) {
    //create the activity
    $ctl = entity_get_controller('elobbyist_activity');
    $activity = $ctl->create(array(
      'mid' => $this->Mailing->mid,
      'tid' => $this->Mailing->tid,
      'fromcid' => $Sender->cid,
      'tocid' => $Recipient->cid,
      ));

    $activity = $ctl->save($activity);

    if (!$activity)
      return FALSE;

    //save the activity as processed for this run
    $this->Activities[$activity->fromcid . ':' . $activity->tocid] = $activity->aid;

    return TRUE;
  }

  protected function LoadActivities() {
    $results = db_select('elobbyist_activity', 'ac')
             ->condition('mid', $this->Mailing->mid)
             ->condition('fromcid', $this->FromIds + $this->CollectedSendersIds, 'IN')
             ->condition('tocid', $this->ToIds, 'IN')
             ->fields ('ac', array (
                    'aid',
                    'fromcid',
                    'tocid',
                  )
              )
             ->execute();
    
    foreach($results as $row) {
      $this->Activities[$row->fromcid . ':' . $row->tocid] = $row->aid;
    }
  }
  
  protected function HasActivity($from, $to) {
    $sKey = $from . ':' . $to;
    return isset($this->Activities[$sKey]);
  }
  
  
  /*
   * Mailing processing finalization
   */
  protected function Close()
  {
    //Once a mailing is completely processed and its end date has passed - change its status to 'closed'
    if ($this->Mailing->schedule == ELOBBYIST_MAILING_SCHEDULE_SIMPLE || $this->Mailing->end < REQUEST_TIME) {
      $this->Mailing->status = ELOBBYIST_MAILING_STATUS_CLOSED;
      entity_get_controller('elobbyist_mailing')->save($this->Mailing);
    }
  }
  
  
  protected function PrepareNotifications() {
    $this->NotificationSenderEmail = variable_get('elobbyist_sender_email');
    if (empty($this->NotificationSenderEmail)) {
      return;
    }
    
    $this->NotificationSenderPass= variable_get('elobbyist_sender_pass');
    if (empty($this->NotificationSenderPass)) {
      return;
    }
    $this->NotificationSenderPass= $this->Decrypt($this->NotificationSenderPass);
    
    $providerid = variable_get('elobbyist_sender_providerid');
    if (empty($providerid)) {
      return;
    }
    $providers = entity_load('elobbyist_email_provider', array($providerid));
    if (!isset($providers) || count($providers) == 0) {
      return;
    }
    reset($providers);
    $provider = current($providers);
    $this->NotificationSenderSmtpHost = $provider->smtphost;
    $this->NotificationSenderSmtpPort = $provider->smtpport;
    
    $this->NotificationSubject = str_replace('[elobbyist:subject]', $this->Mailing->subject,
      elobbyist_get_notify_subject());
    
    $this->NotificationSenderReady = TRUE;
  }
  
  protected function NotifySender($Sender, &$Body, &$RecipientEmailList, &$RecipientNameList) {
    //send notification email
    $oMailResponse = NULL;
    $oEmail = NULL;
    try
    {
      
      $oEmail = new Elobbyist_MailInfo();
      $oEmail->From = $this->NotificationSenderEmail;
      $oEmail->To = $Sender->email;
      $oEmail->Subject = $this->NotificationSubject;
      $oEmail->Body = str_replace(
        array(
          '[elobbyist:recipients emails]',
          '[elobbyist:recipients]',
          '[elobbyist:body]',
          '[elobbyist:unsubscribe]',
          '[elobbyist:subscribe]',
          '[elobbyist:signup]',
          '[elobbyist:remove]',
        ),
        array(
          join(', ', $RecipientEmailList),
          join(', ', $RecipientNameList),
          $Body,
          l(t('Unsubscribe'), url('elobbyist/contact/unsubscribe/'  . $Sender->handle, array('absolute' => TRUE)), 
            array('attributes' => array('target' => '_blank'))),
          l(t('Subscribe'), url('elobbyist/contact/subscribe/' . $Sender->handle, array('absolute' => TRUE)), 
            array('attributes' => array('target' => '_blank'))),
          l(t('Sign up'), url('elobbyist/contact/signup/' . $Sender->handle, array('absolute' => TRUE)), 
            array('attributes' => array('target' => '_blank'))),
          l(t('Remove'), url('elobbyist/contact/remove/' . $Sender->handle, array('absolute' => TRUE)), 
            array('attributes' => array('target' => '_blank'))),
        ),
        elobbyist_get_notify_body()
      );
      $oEmail->Host = $this->NotificationSenderSmtpHost;
      $oEmail->Port = $this->NotificationSenderSmtpPort;
      $oEmail->Password = $this->NotificationSenderPass;
      
      $oMailObj = new Elobbyist_Mail();
      $oMailObj = $this->GetMailObject();
      $oMailResponse = $oMailObj->SendMail($oEmail);

      if (!$oMailResponse->Success) {
        $this->LogMailError(NULL, 'subscriber', $oMailResponse, $oEmail);
      }
    }
    catch(Exception $ex) {
      $this->LogMailError($ex, 'subscriber', $oMailResponse, $oEmail);
    }
  }


  protected function LogMailError($e, $address, $oMailResponse, $oEmail) {
    $str = "Address: $address\n";
    
    if (isset($oEmail)) {
      if (isset($oEmail->From)) {
        $str = " \nSender:" . $oEmail->From;
      }
      if (isset($oEmail->To)) {
        $str = " \nRecipient: " . $oEmail->To;
      }
    }
    if (isset($e)) {
      $str .= " \n" . $ex->getMessage() . " \nTrace: " . $ex->getTraceAsString();
    }
    
    if (isset($oMailResponse)) {
      if (isset($oMailResponse->ErrorMessage)) {
        $str .= " \nMail object error message: " . $oMailResponse->ErrorMessage;
      }

      if (isset($oMailResponse->ErrorCode)) {
        $str .= " \nMail object error code: " . $oMailResponse->ErrorCode;
      }
    }
    
    watchdog('elobbyist cron', $str, array(), WATCHDOG_ERROR);
  }
  
  /*
   * Prepare all entity ids for the main process
   */
  protected function ProcessRelations() {
    $this->ProcessMailingRelations();
    $this->ProcessGroupsToContactsRelations($this->FromGroups, $this->FromIds, 'elobbyist_subscriber');
    $this->ProcessGroupsToContactsRelations($this->ToGroups, $this->ToIds, 'elobbyist_recipient');
    if ($this->Mailing->schedule == ELOBBYIST_MAILING_SCHEDULE_ON_GOING && !empty($this->Mailing->addtogrpid)) {
      $this->ProcessCollectedSendersRelations();
    }
  }
  
  protected function ProcessGroupsToContactsRelations(&$groups, &$target, $relation_type) {
    //from groups can be empty
    if (count($groups) == 0) {
      return;
    }
    $query = new RelationQuery();
    $query->fieldCondition('endpoints', 'entity_type', 'elobbyist_group', '=');
    $query->fieldCondition('endpoints', 'entity_id', $groups, 'IN');
    $relations = entity_load('relation', array_keys($query->execute()));

    foreach($relations as $relation) {
      if ($relation->relation_type != $relation_type) {
        continue;
      }
      foreach($relation->endpoints as $endpoints) {
        foreach($endpoints as $endpoint) {
          if ($endpoint['entity_type'] == 'elobbyist_contact') {
            $target[] = $endpoint['entity_id'];
          }
        }
      }
    }
  }
  
  protected function ProcessCollectedSendersRelations() {
    $groups = entity_load('elobbyist_group', array($this->Mailing->addtogrpid));
    if (!isset($groups) || count($groups) == 0) {
      return;
    }
    reset($groups);
    $group = current($groups);
    if ($group->status == ELOBBYIST_GROUP_STATUS_INACTIVE) {
      return;
    }
    
    $relations = elobbyist_relations_get_data('elobbyist_group', $this->Mailing->addtogrpid);
    
    foreach($relations as $relation) {
      if ($relation->relation_type != 'elobbyist_subscriber') {
        continue;
      }
      foreach($relation->endpoints as $endpoints) {
        foreach($endpoints as $endpoint) {
          if ($endpoint['entity_type'] == 'elobbyist_contact') {
            $this->CollectedSendersIds[] = $endpoint['entity_id'];
          }
        }
      }
    }
  }
  
  /*
   * Fill from and to arrays with group ids
   */
  protected function ProcessMailingRelations() {
    $bFrom = FALSE;
    $bTo = FALSE;
      
    $relations = elobbyist_relations_get_data('elobbyist_mailing', $this->Mailing->mid);
    foreach($relations as $relation) {
      $bFrom = FALSE;
      $bTo = FALSE;
      if ($relation->relation_type == 'elobbyist_from') {
        $bFrom = TRUE;
      }
      elseif ($relation->relation_type == 'elobbyist_to') {
        $bTo = TRUE;
      }
      foreach($relation->endpoints as $endpoints) {
        foreach($endpoints as $endpoint) {
          //add each group endpoint
          if ($endpoint['entity_type'] == 'elobbyist_group') {
            if ($bFrom) {
              $this->FromGroups[$endpoint['entity_id']] = $endpoint['entity_id'];
            }
            elseif ($bTo) {
              $this->ToGroups[$endpoint['entity_id']] = $endpoint['entity_id'];
            }
          }
        }
      }
    }
    
    //remove inactive groups
    $this->RemoveInactiveGroups($this->FromGroups);
    $this->RemoveInactiveGroups($this->ToGroups);
    
  }
  
  /*
   * Remove groups with inactive status
   */
  protected function RemoveInactiveGroups(&$grparr) {
    if (count($grparr) == 0) {
      return;
    }
    
    $groups = entity_load('elobbyist_group', $grparr);
    foreach($groups as $grpid => $group) {
      if ($group->status == ELOBBYIST_GROUP_STATUS_INACTIVE) {
        unset($grparr[$grpid]);
      }
    }
  }
  
  protected function Decrypt($str) {
    return decrypt(base64_decode($str), array('base64' => TRUE));
  }
  
  protected function GetMailObject() {
    return new Elobbyist_Mail();
  }
  
  protected function SkipSender($Sender) {
    //if subscriber is cancelled or bouncing, skip it
    if ($Sender->stype == ELOBBYIST_SUBSCRIBER_TYPE_CANCELLED ||
           $Sender->bounce == ELOBBYIST_CONTACT_EMAIL_BOUNCING_IGNORED) {
      return TRUE;
    }
    
    //skip, if the current mailing is not in the subscriber's specific mailings
    if ($Sender->stype == ELOBBYIST_SUBSCRIBER_TYPE_MAILING) {
      $Mailings = field_get_items('elobbyist_contact', $Sender, 'elobbyist_subscriber_mailing');
      if (!$Mailings){ 
        return TRUE; 
      }
      
      foreach($Mailings as $Mailing) {
        if ($Mailing['target_id'] == $this->Mailing->mid) {
          return FALSE; //mailing found, process it (do not skip)
        }
      }
      return TRUE; //mailing not found, so skip it
    }
    
    return FALSE; //do not skip persistent subscribers that are not bouncing
  }
}

?>
