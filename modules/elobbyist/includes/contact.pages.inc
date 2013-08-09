<?php

/**
 * @file
 * Contact entity pages callbacks
 */

/*
 * forms callback
 */
function elobbyist_contact_form($form, &$form_state, $contact) {
  // Set the id to identify this as a contact edit form.
  $form['#id'] = 'elobbyist_contact-form';
    
  drupal_add_css(drupal_get_path('module', 'elobbyist') . 
    '/includes/elobbyist.css', array('type' => 'file'));
    
  // Save the contact for later, in case we need it.
  $form_state['elobbyist_contact'] = $contact;
  
  //save hidden/disabled values for this form
  $form_state['values']['stype'] = $contact->stype;
  $form_state['values']['form'] = $contact->form;
  
  $new_record = empty($contact->cid);
  
  // Set fields
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Full Name'),
    '#default_value' => $contact->name,
    '#weight' => -5,
    '#maxlength' => 100,
    '#size' => 50,
    '#required' => TRUE,
  );
  
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#default_value' => $contact->email,
    '#weight' => 3,
    '#maxlength' => 254,
    '#size' => 50,
    '#required' => TRUE,
    '#rules' => array('email'),
  );
  
    
  if ($contact->type == 'subscriber') {
    
    elobbyist_add_js_file('subscriber.js');
    
    $form['email']['#ajax'] = array(
      'callback' => 'elobbyist_email_change_callback',
      'wrapper' => 'email_change_div',
      );
    
    $form['email_change_markup'] = array(
      '#markup' => '<div id="email_change_div"></div>',
      );
                   
    //subscriber specific fields
    
    //password
    $form['pass'] = array(
      '#type' => 'password',
      '#title' => t('Password'),
      //never send back the password, so no default_value
      '#default_value' => '',
      '#weight' => 4,
      '#maxlength' => 64,
      '#size' => 50,
      '#description' => t('Email account password.'),
      '#required' => $new_record,
      );
    
    //provider
    $form['providerid'] = array(
      '#type' => 'select',
      '#key_type' => 'associative',
      '#title' => t('Email Provider'),
      '#options' => array('' => '') + elobbyist_get_email_providers(),
      '#default_value' => $contact->providerid,
      '#weight' => 20,
      '#description' => t('Select the Email provider for the email account.'),
      '#required' => TRUE,
    );
    
    //bounce
    $form['bounce'] = array(
      '#type' => 'select',
      '#title' => t('Email Status'),
      '#options' => elobbyist_email_statuses(),
      '#default_value' => $contact->bounce,
      '#weight' => 25,
      '#description' => t('Normally this value is only updated by the cron job, which updates it to verfied/bouncing according to email send error/success.'),
      '#required' => TRUE,
    );
    
    if (!empty($contact->bounces)) {
      $form['bounces'] = array(
        '#markup' => theme('elobbyist_field',
          array(
            'title' => t('Number of consequent email send bounces'),
            'item' => $contact->bounces,
          )
        ),
        '#weight' => 28,
      );
    }
    
    $form['notify'] = array(
      '#type' => 'checkbox',
      '#title' => t('Notify'),
      '#default_value' => $contact->notify,
      '#weight' => 30,
      '#description' => t('Notify the contact on each email sent.'),
    );
    
    $form['stype'] = array(
      '#type' => 'select',
      '#title' => t('Subscription'),
      '#options' => elobbyist_subscriber_types(),
      '#default_value' => $contact->stype,
      '#weight' => 33,
      '#description' => t('This field is normally not changed manually. The \'Specific Mailing\' option is used in on going schedule mailings to collect per-action contacts. 
        Only persistent subscribers incldued in a mailing \'from\' field are processed.'),
      '#required' => TRUE,
    );  
  }
  
  $relation_type = 'elobbyist_' . $contact->type;
  
  $form[$relation_type] = array(
      '#type' => 'elobbyist_contact_groups',
      '#relation_type' => $relation_type,
      '#title' => t('Groups'),
      '#weight' => 40,
      '#description' => t('Emails are only sent between groups.'),
  );
  
  //Add the buttons
  $form['buttons'] = array();
  $form['buttons']['#weight'] = 100;
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 100,
    '#submit' => array('elobbyist_contact_form_submit'),
  );
  
  $form['buttons']['save_and_add'] = array(
    '#type' => 'submit',
    '#value' => t('Save and Add'),
    '#weight' => 110,
    '#submit' => array('elobbyist_contact_form_submit', 'elobbyist_contact_redirect_to_add'),
  );
  
  if (!$new_record) {
    $form['buttons']['delete'] = array(
      '#access' => user_access('delete email lobbyist contacts'),
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 120,
      '#submit' => array('elobbyist_contact_form_delete_submit'),
      '#limit_validation_errors' => array(),       // No validation.
    );
  }
  
  $form['#validate'][] = 'elobbyist_contact_form_validate';
  
  field_attach_form('elobbyist_contact', $contact, $form, $form_state);
  
  return $form;
}


/*
 * Validate function
 */
function elobbyist_contact_form_validate($form, &$form_state) {
  //skip iterations invoked by ajax
  if (!isset($form_state['submitted']) || !$form_state['submitted'])  {
    return;
  }
  
  $contact = $form_state['elobbyist_contact'];

  
  //encrypt password
  if (isset($form_state['values']['pass'])) {
    $contact->pass = elobbyist_encrypt($form_state['values']['pass']);
    //do not store plain password from now on
    unset($form_state['values']['pass']);
  }
      
  //Field API validation
  field_attach_form_validate('elobbyist_contact', $contact, $form, $form_state);
  
  if (!form_get_errors()) {
    //verify that email is unique
    elobbyist_unique_field('elobbyist_contact', $contact->cid, 'cid', 'email', 
      $form_state['values']['email'], t('Email address'));
  }
  
  //if there are errors, do not send back even encrypted password
  if (form_get_errors()) {
    $contact->pass = '';
  }
}


/*
 * Save functions
 */
function elobbyist_contact_form_submit($form, &$form_state) {
  global $user;
  
  $contact = $form_state['elobbyist_contact'];
  
  $contact->name = $form_state['values']['name'];
  $contact->email = $form_state['values']['email'];
  
  if (isset($form_state['values']['providerid']) && !empty($form_state['values']['providerid'])) {
    $contact->providerid = $form_state['values']['providerid'];
  }
    
  if (isset($form_state['values']['stype']) && !empty($form_state['values']['stype'])) {
    $contact->stype = $form_state['values']['stype'];
  }
  
  if (isset($form_state['values']['notify'])) {
    $contact->notify = $form_state['values']['notify'];
  }
  
  if (isset($form_state['values']['bounce'])) {
    $contact->bounce = $form_state['values']['bounce'];
    //reset count, if bounce is set to unverified
    if ($contact->bounce == ELOBBYIST_CONTACT_EMAIL_UNVERIFIED && $contact->bounces > 0) {
      $contact->bounces = 0;
    }
  }
      
  // Notify field widgets.
  field_attach_submit('elobbyist_contact', $contact, $form, $form_state);
  
  $new_record = empty($entity->cid);
  
  // Save the contact.
  elobbyist_contact_save($contact);
  //empty the password
  
  $form_state['elobbyist_contact'] = $contact; //send back the contact, without password
  
  //if new record, call submit groups manually, because it needs the entity id
  if ($new_record) {
    elobbyist_field_attach_submit('elobbyist_contact', $contact, $form, $form_state);
  }
  
  $types = elobbyist_contact_types();
  
  // Notify the user.
  if ($contact) {
    drupal_set_message(t('@name saved.',array( '@name' => $types[$contact->type]->name)));
    
    if ($new_record) {
      if (entity_access('update', 'elobbyist_contact')) {
        $form_state['redirect'] = 'elobbyist/contact/' . $contact->cid . '/edit';
      }
      elseif (entity_access('view', 'elobbyist_contact')) {
        $form_state['redirect'] = 'elobbyist/contact/' . $contact->cid . '/view';
      }
      else {
        if ($contact->type == 'subscriber') {
          $form_state['redirect'] = 'elobbyist/subscribers/add/subscriber';
        }
        else {
          $form_state['redirect'] = 'elobbyist/recipients/add/recipient';
        }
      }
    }
  }
}

function elobbyist_contact_redirect_to_add($form, &$form_state) {
  $contact = $form_state['elobbyist_contact'];
  
  if ($contact->type == 'subscriber') {
    $form_state['redirect'] = 'elobbyist/subscribers/add/subscriber';
  }
  else {
    $form_state['redirect'] = 'elobbyist/recipients/add/recipient';
  }
}

/*
 * Delete functions
 */
function elobbyist_contact_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  
  $contact = $form_state['elobbyist_contact'];
  $form_state['redirect'] = array('elobbyist/contact/' . $contact->cid . '/delete',
    array('query' => $destination));
}
function elobbyist_contact_delete_confirm($form, &$form_state, $contact) {    
  
  $types = elobbyist_contact_types();
  
  $path = 'elobbyist/' . $types[$contact->type]->group;

  // Always provide entity id in the same form key as in the entity edit form.
  $form['cid'] = array('#type' => 'value', '#value' => $contact->cid);
  
  return confirm_form($form, 
    t('Are you sure you want to delete %title?',
      array('%title' => $contact->name)),
    $path,
    t('This action cannot be undone'),
    t('Delete'), 
    t('Cancel')
  );
}
function elobbyist_contact_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $contact = elobbyist_contact_load($form_state['values']['cid']);
        
    $types = elobbyist_contact_types();
    
    $path = 'elobbyist/' . $types[$contact->type]->group;
    
    elobbyist_contact_delete($form_state['values']['cid']);
        
    watchdog('elobbyist_contact', '@type: deleted %title.' , array('@type' => 
      $contact->type, '%title' => $contact->name) );
    
    
    drupal_set_message(t('@type %title has been deleted.', array(
      '@type' => $types[$contact->type]->name,
      '%title' => $contact->name)
      )
    );
    
    $form_state['redirect'] = $path;
  }
}


function elobbyist_contact_page_view($entity, $view_mode = 'full') {
  
  elobbyist_contact_page_title($entity);
  
  // Remove previously built content, if exists.
  $entity->content = array();
  
  $entity->content['#attached']['css'] = array(
    drupal_get_path('module', 'elobbyist') . '/includes/elobbyist.css',
  );
      
  $entity->content['email'] = array(
    '#markup' => 
      theme('elobbyist_field',
        array(
          'title' => t('Email'),
          'item' => $entity->email,
        )
     ),
    '#weight' => -50,
  );
  
  if ($entity->type == 'subscriber') {
    if (!empty($entity->providerid)) {
      $provider = entity_load('elobbyist_email_provider', array($entity->providerid));
      
      if ($provider != null && is_array($provider)) {
        $entity->content['providerid'] = array(
          '#markup' => theme('elobbyist_field',
              array(
                'title' => t('Provider'),
                'item' => current($provider)->name,
              )
           ),
          '#weight' => -45,
        );
      }
    }
    
    if (isset($entity->bounce)) {
      $email_statuses = elobbyist_email_statuses();
      $entity->content['bounce'] = array(
        '#markup' => theme('elobbyist_field',
            array(
              'title' => t('Email Status'),
              'item' => $email_statuses[$entity->bounce],
            )
         ),
        '#weight' => -40,
      );
    }
    
    if (!empty($entity->bounces)) {
      $entity->content['bounces'] = array(
        '#markup' => theme('elobbyist_field',
          array(
            'title' => t('Number of consequent email send bounces'),
            'item' => $entity->bounces,
          )
        ),
        '#weight' => -38,
      );
    }
  
    $entity->content['notify'] = array(
      '#markup' => theme('elobbyist_field',
          array(
            'title' => t('Notify on each mail'),
            'item' => $entity->notify ? t('Yes') : t('No'),
          )
       ),
      '#weight' => -35,
    );

    if (!empty($entity->form)) {
      $form_types = elobbyist_form_types();
      $entity->content['form'] = array(
        '#markup' => theme('elobbyist_field',
            array(
              'title' => t('Created through'),
              'item' => $form_types[$entity->form],
            )
         ),
        '#weight' => -30,
      );
    }
    
    if (!empty($entity->stype)) {
      $stypes = elobbyist_subscriber_types();

      $entity->content['stype'] = array(
        '#markup' => theme('elobbyist_field',
                array(
                  'title' => t('Subscription'),
                  'item' => $stypes[$entity->stype] ,
                )
             ),
        '#weight' => -27,
      );
    }
  }
  
  $format_short_date = variable_get('date_format_short', 'Y-n-j - H:i');
  
  $entity->content['created'] = array(
    '#markup' => theme('elobbyist_field',
          array(
            'title' => t('Created on'),
            'item' => format_date($entity->created, 'custom', $format_short_date),
          )
       ),
      '#weight' => -16,
  );
  
  $entity->content['changed'] = array(
    '#markup' => theme('elobbyist_field',
          array(
            'title' => t('Modified on'),
            'item' => format_date($entity->changed, 'custom', $format_short_date),
          )
       ),
      '#weight' => -15,
  );
  
  // Build fields content.
  field_attach_prepare_view('elobbyist_contact', 
    array($entity->cid => $entity), $view_mode);
  
  entity_prepare_view('elobbyist_contact', array($entity->cid => $entity));
  
  $entity->content += field_attach_view('elobbyist_contact', $entity, $view_mode);
  
  return $entity->content;
}

function elobbyist_contact_emails($entity, $view_mode = 'full') {
  drupal_set_title(t('@contact\'s Emails', array('@contact' => $entity->name)));
 
  $view = NULL;
  if ($entity->type == 'subscriber') {
    $view = views_get_view('elobbyist_activities_by_subscriber');
  }
  else {
    $view = views_get_view('elobbyist_activities_by_recipient');
  }

  return $view->preview('default', array($entity->cid));
}

/*
 * Add record page callback
 */
function elobbyist_contact_add($type) {
  $types = elobbyist_contact_types();
  $type  = (isset($type) ? str_replace('-', '_', $type) : NULL);
  if (empty($types[$type])) {
    return MENU_NOT_FOUND;
  }
  
  $contact = entity_get_controller('elobbyist_contact')->create(array('type' => $type));
  
  //set subscription type to persistent for subscribers
  if ($type == 'subscriber') {
    $contact->stype = ELOBBYIST_SUBSCRIBER_TYPE_PERSISTENT;
    $contact->form = ELOBBYIST_FORM_ADMIN;
    $contact->notify = variable_get('elobbyist_notify_default_admin', ELOBBYIST_DEFAULT_NOTIFY_ON_EMAILS_SENT_FOR_ADMIN_FORM);
    $contact->bounce = ELOBBYIST_CONTACT_EMAIL_UNVERIFIED;
  }  
  
  drupal_set_title(t('New @name', array('@name' => $types[$type]->name)),
    PASS_THROUGH);
  
  return drupal_get_form('elobbyist_contact_form', $contact);
}

/*
 * Edit record page callback
 */
function elobbyist_contact_page_edit($contact) {
  elobbyist_contact_page_title($contact);
  
  return drupal_get_form('elobbyist_contact_form', $contact);
}

/*
 * Build the call-to-action public page
 * 
 * For the page to be displayed - a mailing must be active, of schedule type on-going and start date has past
 * If not displayed: a general "Oops! the page you requested cannot be found" message is to be shown
 * 
 * For the page to receive subscribers, end date has yet to pass, taking into account also a setting of X minutes/hours even before end date
 */
function elobbyist_mailing_public_page($entity) {
  
  $bExit = FALSE;
  $bSubscriptionForm = FALSE;
  
  if (!isset($entity) || 
      !isset($entity->schedule) || 
      $entity->schedule != ELOBBYIST_MAILING_SCHEDULE_ON_GOING) {
    $bExit = TRUE;
  }
  else {
    switch($entity->status) {
      case ELOBBYIST_MAILING_STATUS_ACTIVE:
        if ($entity->start > REQUEST_TIME) {
          if (!user_access('edit email lobbyist mailings')) {
            $bExit = TRUE;
             break;
          }
          else {
            drupal_set_message(t('The page is not published, as its mailing has not started yet.'), 'warning');
            break;
          }
        }
        //user should have anonymous access
        if (!user_access('view email lobbyist published pages')) {
          $bExit = TRUE;
          break;
        }        
        if (REQUEST_TIME < $entity->end) {
          $bSubscriptionForm = TRUE;
        }
        else {
          drupal_set_message(t('This action has ended.'), 'warning');
        }
        break;
      case ELOBBYIST_MAILING_STATUS_DRAFT:
      case ELOBBYIST_MAILING_STATUS_CLOSED:
      case ELOBBYIST_MAILING_STATUS_CANCELLED:
        if (!user_access('edit email lobbyist mailings')) {
          $bExit = TRUE;
           break;
        }
        $statuses = elobbyist_mailing_status_list();
        drupal_set_message(t('The page is not published, as its mailing is in status @status.',
          array('@status' => $statuses[$entity->status])
          ), 'warning');
        break;
      default:
        //unknown status
        $bExit = TRUE;
        break;
    }
  }
  
  if ($bExit) {
    drupal_set_title(t('Oops! The page you requested cannot be found'));
    return array();
  }

  drupal_set_title($entity->name, PASS_THROUGH);

  // Remove previously built content, if exists.
  $entity->content = array();
    
  $entity->content['page'] = array(
    '#markup' => $entity->page,
    '#weight' => -50,
  );
  
  if ($bSubscriptionForm) {
    $contact = entity_get_controller('elobbyist_contact')->create(array('type' => 'subscriber',
      'form' => ELOBBYIST_FORM_PUBLIC,
      'notify' => variable_get('elobbyist_notify_default_public', ELOBBYIST_DEFAULT_NOTIFY_ON_EMAILS_SENT_FOR_PUBLIC_FORM),
      'bounce' => ELOBBYIST_CONTACT_EMAIL_UNVERIFIED
      ));
        
    return drupal_get_form('elobbyist_contact_public_form', $entity, $contact);
  }
  else {
    return $entity->content;
  }
}

/*
 * Prepares a confirm page for removal of a contact from mailings by setting its subscription type to Specific Mailing
 */
function elobbyist_contact_remove_confirm($handle) {
  drupal_set_title(t('Remove Subscription'), PASS_THROUGH);
  return drupal_get_form('elobbyist_contact_remove_confirm_form', $handle);  
}

/*
 * Confirm page for removal of a contact from mailings by setting its subscription type to Specific Mailing
 * 
 * Create a form like the one created by confirm_form(), just not as a follow-up form
 */
function elobbyist_contact_remove_confirm_form($form, &$form_state, $handle) {
  drupal_set_title(t('Are you sure you want to remove your subscription?'), PASS_THROUGH);

  $form['#attributes']['class'][] = 'confirmation';
  $form['description'] = array('#markup' => t('Emails will not be sent on your behalf by the site admins.'));
  $form['confirm'] = array('#type' => 'hidden', '#value' => 1);
  $form['handle'] = array('#type' => 'value', '#value' => $handle);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Remove'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => '',
    '#options' => array('path' => ''),
  );
  $form['#theme'] = 'confirm_form';

  return $form;
}

/*
 * Removes a contact from mailings by setting its subscription type to Specific Mailing
 */
function elobbyist_contact_remove_confirm_form_submit($form, &$form_state) {
  
  $handle = $form_state['values']['handle'];
  $contact = elobbyist_subscriber_by_handle($handle);
  
  if (!$contact) {
    drupal_set_message(t('Error! The link you have used is broken or invalid.'), 'error');
    $form_state['redirect'] = '';
    return;
  }
  
  //remove the subscription by setting the subscriber to a specific mailing and unsetting the mailing
  $contact->stype = ELOBBYIST_SUBSCRIBER_TYPE_CANCELLED;
  if (elobbyist_contact_save($contact)) {
    drupal_set_message(t('Success! Your subscription was removed successfully.'), 'status');
  }
  else {
    drupal_set_message(t('Error! There\'s been an error in removing your subscription. Please try again later.'), 'error');
  }
  
  $form_state['redirect'] = '';
}

function elobbyist_contact_signup_confirm($handle) {
  drupal_set_title(t('Sign up'), PASS_THROUGH);
  return drupal_get_form('elobbyist_contact_signup_confirm_form', $handle);
}

function elobbyist_contact_signup_confirm_form($form, &$form_state, $handle) {
  drupal_set_title(t('Are you sure you want to sign up?'), PASS_THROUGH);

  $form['#attributes']['class'][] = 'confirmation';
  $form['description'] = array('#markup' => t('Emails will be sent on your behalf by the site admins.'));
  $form['confirm'] = array('#type' => 'hidden', '#value' => 1);
  $form['handle'] = array('#type' => 'value', '#value' => $handle);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Sign up'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => '',
    '#options' => array('path' => ''),
  );
  $form['#theme'] = 'confirm_form';

  return $form;
}

function elobbyist_contact_signup_confirm_form_submit($form, &$form_state) {
  $handle = $form_state['values']['handle'];
  $contact = elobbyist_subscriber_by_handle($handle);
  
  if (!$contact) {
    drupal_set_message(t('Error! The link you have used is broken or invalid.'), 'error');
    $form_state['redirect'] = '';
    return;
  }
    
  //make subscriber persistent, so ze will beable to participate in all actions
  $contact->stype = ELOBBYIST_SUBSCRIBER_TYPE_PERSISTENT;
  
  if (elobbyist_contact_save($contact)) {
    drupal_set_message(t('Success! Your have signed up successfully.'), 'status');
  }
  else {
    drupal_set_message(t('Error! There\'s been an error in restoring your subscription. Please try again later.'), 'error');
  }
  
  $form_state['redirect'] = '';
}

/*
 * Stops notifications for the contact. No confirmation is required
 */
function elobbyist_contact_unsubscribe($handle) {
  $contact = elobbyist_subscriber_by_handle($handle);
  
  if (!$contact) {
    drupal_set_message(t('Error! The link you have used is broken or invalid.'), 'error');
    return array();
  }
  
  $links = ELOBBYIST_SUBSCRIBER_LINK_SUBSCRIBE;
  
  $contact->notify = 0;
  if (elobbyist_contact_save($contact)) {
    drupal_set_title(t('Unsubscribed.'), PASS_THROUGH);
    drupal_set_message(t('Success! Your have been unsubscribed successfully.'), 'status');
    
    if ($contact->stype == ELOBBYIST_SUBSCRIBER_TYPE_PERSISTENT) {
      $links |= ELOBBYIST_SUBSCRIBER_LINK_REMOVE;
    }
    else {
      $links |= ELOBBYIST_SUBSCRIBER_LINK_SIGNUP;
    }
    
    $content = array();
    $content['note'] = array(
      '#markup' => '<p>' . t('You will not receive notification emails from this site.') . '</p>',
      '#weight' => -10,
    );
    return $content + elobbyist_contact_public_links($handle, $links);
  }
  else {
    drupal_set_message(t('Error! There\'s been an error in unsubscribing you. Please try again later.'), 'error');
    return array();
  }
}

function elobbyist_contact_subscribe($handle) {
  $contact = elobbyist_subscriber_by_handle($handle);
  
  if (!$contact) {
    drupal_set_message(t('Error! The link you have used is broken or invalid.'), 'error');
    return array();
  }
  
  $links = ELOBBYIST_SUBSCRIBER_LINK_UNSUBSCRIBE;
  
  $contact->notify = 1;
  if (elobbyist_contact_save($contact)) {
    drupal_set_title(t('Subscribed.'), PASS_THROUGH);
    drupal_set_message(t('Success! Your have been subscribed successfully.'), 'status');
    
    if ($contact->stype == ELOBBYIST_SUBSCRIBER_TYPE_PERSISTENT) {
      $links |= ELOBBYIST_SUBSCRIBER_LINK_REMOVE;
    }
    else {
      $links |= ELOBBYIST_SUBSCRIBER_LINK_SIGNUP;
    }
    
    $content = array();
    $content['note'] = array(
      '#markup' => '<p>' . t('You will now receive a notification email from this site on each email sent on your behalf.') . '</p>',
      '#weight' => -10,
    );
    return $content + elobbyist_contact_public_links($handle, $links);
  }
  else {
    drupal_set_message(t('Error! There\'s been an error in subscribing you. Please try again later.'), 'error');
    return array();
  }
}

function elobbyist_contact_public_links($handle, $bitwise) {
  $links = array();
  if ($bitwise & ELOBBYIST_SUBSCRIBER_LINK_SIGNUP) {
    $links['signup'] = array(
      '#markup' => '<div>' . l(t('Participate in all this site actions'), 'elobbyist/contact/signup/' . $handle)
                    . '<p>' . t('Allow the site admins to send emails on your behalf.') . '</p></div>',
    );
  }
  if ($bitwise & ELOBBYIST_SUBSCRIBER_LINK_REMOVE) {
    $links['remove'] = array(
      '#markup' => '<div>' . l(t('Stop participating in all this site actions'), 'elobbyist/contact/remove/' . $handle)
                  . '<p>' . t('Site admins will no longer send emails on your behalf.') . '</p></div>',
    );
  }
  if ($bitwise & ELOBBYIST_SUBSCRIBER_LINK_SUBSCRIBE) {
    $links['subscribe'] = array(
      '#markup' => '<div>' . l(t('Subscribe'), 'elobbyist/contact/subscribe/' . $handle)
                  . '<p>' . t('Receive a notification on each email sent on your behalf by the site admins.') . '</p></div>',
    );
  }
  if ($bitwise & ELOBBYIST_SUBSCRIBER_LINK_UNSUBSCRIBE) {
    $links['unsubscribe'] = array(
      '#markup' => '<div>' . l(t('Unsubscribe'), 'elobbyist/contact/unsubscribe/' . $handle)
                  . '<p>' . t('Do not receive a notification on emails sent on your behalf by the site admins.') . '</p></div>',
    );
  }
  
  return $links;
}

function elobbyist_contact_page_title($contact) {
  $types = elobbyist_contact_types();
  drupal_set_title(t('@type: @title', array(
    '@type' => $types[$contact->type]->name,
    '@title' => $contact->name, )), PASS_THROUGH);
}


function elobbyist_subscriber_by_handle(&$handle) {
  $query = db_select('elobbyist_contact', 'c')
          ->condition('handle', $handle)
          ->condition('type', 'subscriber')
          ->fields('c', array('cid'))
          ->execute();
  $col =  $query->fetchColumn();

  if (isset($col)) {
    $contacts = entity_load('elobbyist_contact', array($col));
    if (isset($contacts) && count($contacts) > 0) {
      return current($contacts);
    }
  }
  return FALSE;
}