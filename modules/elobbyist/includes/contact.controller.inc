<?php

/*
 * contact entity controller
 * 
 */
class ELobbyistContactController extends EntityAPIControllerExportable {
  
  public function create(array $values = array()) {
    return (object)array_merge(array(
      'cid' => NULL,
      'type' => NULL,
      'name' => NULL,
      'email' => NULL,
      'stype' => NULL,
      'providerid' => NULL,
      'notify' => NULL,
      'handle' => NULL,
      'form' => NULL,
      'bounce' => NULL,
      'bounces' => NULL,
      'pass' => NULL,
    ), $values);
  }
  
  public function save($contact) {
    $transaction = db_transaction();
    
    try {
      global $user;
      
      // Determine if we will be inserting a new record
      $contact->is_new = empty($contact->cid);
      
      // Set the timestamp fields.
      if (empty($contact->created)) {
        $contact->created = REQUEST_TIME;
      }
      
      $contact->changed = REQUEST_TIME;
            
      // Give modules the opportunity to prepare field data for
      // saving.
      field_attach_presave('elobbyist_contact', $contact);
      
      //do not delete password for not being changed (password is never loaded back)
      if (empty($contact->pass))
        unset($contact->pass);
      
      // If this is a new record...
      if ($contact->is_new) {
        // Save the new record
        $contact->handle = base64_encode(md5(uniqid(base64_encode($contact->email), true)));
        drupal_write_record('elobbyist_contact', $contact);
                
        $op = 'insert';
      }
      else {
        // Save the updated record
        drupal_write_record('elobbyist_contact', $contact, 'cid');
        
        $op = 'update';
      }
            
      // Save fields.
      $function = 'field_attach_' . $op;
      $function('elobbyist_contact', $contact);
      
      module_invoke_all('entity_' . $op, $contact, 'elobbyist_contact');
     
      //clear internal properties
      unset($contact->is_new);
      
      // Ignore slave server temporarily to give time for the saved
      // order to be propagated to the slave.
      db_ignore_slave();

      return $contact;
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('elobbyist_contact', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }
  
  public function delete($cids) {
    if (empty($cids)) {
      return TRUE;
    }
      
    $contacts = $this->load($cids, array());
    $transaction = db_transaction();
    
    try
    {
      db_delete('elobbyist_contact')
        ->condition('cid', $cids, 'IN')
        ->execute();

      foreach($contacts as $contact) {        
        field_attach_delete('elobbyist_contact', $contact);

        module_invoke_all('entity_delete', $contact, 'elobbyist_contact');
      }
      
      if ($contact->type == 'subscriber') {
        elobbyist_foreign_key_cascade_delete('elobbyist_activity', 'fromcid', 'aid', $cids);
      }
      elseif ($contact->type == 'recipient') {
        elobbyist_foreign_key_cascade_delete('elobbyist_activity', 'tocid', 'aid', $cids);
      }

      db_ignore_slave();
    }
    catch(Exception $e) {
      $transaction->rollback();
      watchdog_exception('elobbyist_contact', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
    
    return TRUE;
  }
    
}