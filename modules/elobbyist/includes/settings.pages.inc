<?php

/*
 * Settings page
 * 
 * To add a new setting:
 * 1. Add it as a form element to elobbyist_settings_form
 * 2. Add a line to elobbyist_settings_restore
 * 3. Don't forget to implement the use of the setting in the relavant places
 * 
 * The rest should work automatically!
 */

function elobbyist_settings_form() {  
  
  drupal_set_title(t('Email Lobbyist Settings'), PASS_THROUGH);
  
  $form['notifications'] = array(
    '#type' => 'fieldset',
    '#title' => t('Notifications'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => -20,
    '#description' => t('Notifications sent to subscribers by the site\'s cron job on each email sent on their behalf. Subscription to notifications is controlled by the subscriber\'s Notify field.'),
  );
  
  $form['notifications']['elobbyist_sender_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender Email'),
    '#default_value' => variable_get('elobbyist_sender_email'),
    '#weight' => 0,
    '#maxlength' => 254,
    '#size' => 50,
    '#required' => FALSE,
    '#rules' => array('email'),
    '#description' => t('The email account the notifications will be sent from.'),
  );
  
  $form['notifications']['elobbyist_sender_pass'] = array(
      '#type' => 'password',
      '#title' => t('Sender Password'),
      //never send back the password, so no default_value
      '#default_value' => '',
      '#weight' => 4,
      '#maxlength' => 64,
      '#size' => 50,
      '#description' => t('Email account password.'),
      );
  
  $form['notifications']['elobbyist_sender_providerid'] = array(
      '#type' => 'select',
      '#key_type' => 'associative',
      '#title' => t('Email Provider'),
      '#options' => array('' => '') + elobbyist_get_email_providers(),
      '#default_value' => variable_get('elobbyist_sender_providerid'),
      '#weight' => 5,
      '#description' => t('Email provider for the sender.'),
      '#required' => FALSE,
    );
  
  $form['notifications']['elobbyist_notify_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Notification Subject'),
    '#size' => 100,
    '#max_length' => 255,
    '#default_value' => elobbyist_get_notify_subject(),
    '#description' => t('The subject of the notification email that is sent to a sender that is to be notified on each mail sent. Use [elobbyist:subject] to include the subject of the sent email.'),
    '#weight' => 10,
  );
  
  $form['notifications']['elobbyist_notify_body'] = array(
    '#type' => 'text_format',
    '#title' => t('Notification Body'),
    '#default_value' => elobbyist_get_notify_body(),
    '#weight' => 20,
    '#description' => t('The body of the notification email that is sent to a sender that is to be notified on each mail sent. <br/>
     Allowed tokens: <br/>[elobbyist:body] - original message body <br/>
     [elobbyist:recipients] - recipients names <br/>
     [elobbyist:recipients emails] - recipients email addresses <br/>
     [elobbyist:unsubscribe] - link to stop notifications <br/>
     [elobbyist:subscribe] - link to subscribe to notifications <br/>
     [elobbyist:signup] - link to sign up to all mailings <br/>
     [elobbyist:remove] - link to remove subscription/participation in all mailings <br/>
     Note: These links will only work if the appropriate permissions were granted to the anonymous user role (by default they are granted).'),
    '#required' => TRUE,
  );

  $form['defaults'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default form values'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 0,
    '#tree' => TRUE,
  );

  $form['defaults']['elobbyist_smtp_port_default'] = array(
    '#type' => 'textfield',
    '#title' => t('SMTP port'),
    '#size' => 5,
    '#max_length' => 5,
    '#default_value' => variable_get('elobbyist_smtp_port_default', ELOBBYIST_DEFAULT_SMTP_PORT),
    '#description' => t('Email Provider form'),
    '#weight' => 0,
    '#rules' => array('numeric'),
    '#number_type' => 'integer',
  );

  $form['defaults']['elobbyist_notify_default_public'] = array(
    '#type' => 'checkbox',
    '#title' => t('Notify on Emails Sent'),
    '#default_value' => variable_get('elobbyist_notify_default_public', ELOBBYIST_DEFAULT_NOTIFY_ON_EMAILS_SENT_FOR_PUBLIC_FORM),
    '#description' => t('Subscriber\'s public form'),
    '#weight' => 5,
  );
    
  $form['defaults']['elobbyist_notify_default_admin'] = array(
    '#type' => 'checkbox',
    '#title' => t('Notify on Emails Sent'),
    '#default_value' => variable_get('elobbyist_notify_default_admin', ELOBBYIST_DEFAULT_NOTIFY_ON_EMAILS_SENT_FOR_ADMIN_FORM),
    '#description' => t('Subscriber\'s admin form'),
    '#weight' => 6,
  );
  
  $form['defaults']['elobbyist_notify_default_block'] = array(
    '#type' => 'checkbox',
    '#title' => t('Notify on Emails Sent'),
    '#default_value' => variable_get('elobbyist_notify_default_block', ELOBBYIST_DEFAULT_NOTIFY_ON_EMAILS_SENT_FOR_BLOCK),
    '#description' => t('Subscription block called @name', array('@name' => t('Register Email Lobbyist new persistent subscribers'))),
    '#weight' => 7,
  );
  
  $form['elobbyist_max_emails_per_batch'] = array(
    '#type' => 'textfield',
    '#title' => t('Max emails per batch'),
    '#size' => 7,
    '#max_length' => 7,
    '#default_value' => variable_get('elobbyist_max_emails_per_batch', ELOBBYIST_DEFAULT_MAX_EMAILS_PER_BATCH),
    '#description' => t('After this number of emails the cron job stops, even if a mailing wasn\'t completely processed. The next cron job run proceeds from the same place the previous one has stopped. Zero means all mailings are always processed in whole.'),
    '#required' => TRUE,
    '#weight' => 3,
    '#rules' => array('numeric'),
    '#number_type' => 'integer',
  );
  
  $form['elobbyist_max_bounces'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Bounces'),
    '#size' => 4,
    '#max_length' => 4,
    '#default_value' => variable_get('elobbyist_max_bounces', ELOBBYIST_DEFAULT_MAX_BOUNCES),
    '#description' => t('After this number of sends an email account will be ignored, unless the contact\'s Email Status is changed manually to \'Unverified\' - in which case the count will be reset. Set to zero to never ignore bouncing email accounts.'),
    '#required' => TRUE,
    '#weight' => 5,
    '#rules' => array('numeric'),
    '#number_type' => 'integer',
  );
  
  $form['elobbyist_records_per_page'] = array(
    '#type' => 'select',
    '#title' => t('Records per page'),
    '#default_value' => variable_get('elobbyist_records_per_page', ELOBBYIST_DEFAULT_RECORDS_PER_PAGE),
    '#cache_invalidator' => TRUE,
    '#options' => elobbyist_config_records_per_page_options(),
    '#description' => t('Records per page to display on grids/tables. Changing this value flushes Drupal\'s cache.'),
    '#required' => TRUE,
    '#weight' => 10,
  );

  $form['elobbyist_main_menu_weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Main menu weight'),
    '#default_value' => variable_get('elobbyist_main_menu_weight', ELOBBYIST_DEFAULT_MAIN_MENU_WEIGHT),
    '#cache_invalidator' => TRUE,
    '#size' => 4,
    '#max_length' => 4,
    '#description' => t('Sets the location of the Email Lobbyist main menu inside Drupal\'s menu toolbar. 
      Changing this value flushes Drupal\'s cache.'),
    '#required' => TRUE,
    '#weight' => 50,
    '#rules' => array('numeric'),
    '#number_type' => 'integer',
  );

  $form['buttons'] = array();
  $form['buttons']['#weight'] = 200;
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#weight' => 10,
    '#submit' => array('elobbyist_settings_form_submit'),
  );
  
  $form['buttons']['restore'] = array(
    '#type' => 'submit',
    '#value' => t('Restore defaults'),
    '#weight' => 20,
    '#submit' => array('elobbyist_settings_form_restore_submit'),
    '#limit_validation_errors' => array(),       // No validation.
  );
  
  $form['elobbyist_do_not_flash_cache'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not flush Drupal\'s cache'),
    '#description' => t('Do not flush Drupal\'s cache even when settings that require flushing to take effect have changed. Manual cache flushing will be needed for those settings to take affect. This form does not flush Drupal\'s cache when such settings have not changed.'),
    '#default_value' => ELOBBYIST_DEFAULT_DO_NOT_FLUSH_DRUPAL_CACHE,
    '#weight' => 210,
  ); 
  
  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('The settings have not been saved because of the errors.'), 'error');
  }

  //render the form using theme_system_settings_form().
  if (!isset($form['#theme'])) {
    $form['#theme'] = 'system_settings_form';
  }

  return $form;
}

/**
* Implements validation from the Form API.
*
* @param $form
*   A structured array containing the elements and properties of the form.
* @param $form_state
*   An array that stores information about the form's current state
*   during processing.
*/
function elobbyist_settings_form_validate($form, &$form_state){
  
  elobbyist_settings_validate_negative($form_state['values']['elobbyist_max_bounces'],
    'elobbyist_max_bounces', t('Max Bounces'));
  elobbyist_settings_validate_negative($form_state['values']['elobbyist_max_emails_per_batch'], 
    'elobbyist_max_emails_per_batch', t('Max emails per batch'));
  elobbyist_settings_validate_negative($form_state['values']['defaults']['elobbyist_smtp_port_default'], 
    'elobbyist_smtp_port_default', t('SMTP port'));
}

function elobbyist_settings_validate_negative($field, $fieldname, $label) {
  if (isset($field)) {
    if (0 + $field < 0) {
      form_set_error($fieldname, t('@field does not accept negative numbers.',
        array('@field' => $label)));
    }
  }
}

/**
 * Submits the settings form.
 *
 */
function elobbyist_settings_form_submit($form, &$form_state) {
  // Exclude unnecessary elements.
  form_state_values_clean($form_state);

  if (elobbyist_settings_form_set_values($form, $form_state['values']))
    drupal_flush_all_caches();

  drupal_set_message(t('The configuration options have been saved.'));
}

/*
 * Handles settings save, including inside fieldsets (by recurse)
 * 
 * Since this is an admin form, it's ok to pass arrays by value
 * 
 * Returns True when need to flush the cache
 */
function elobbyist_settings_form_set_values($form, $values) {
  $cache_clear = FALSE;
  $cache_unclear = FALSE;
  $value_to_set = NULL;
    
  foreach ($values as $key => $value) {

    switch($key) {
      case 'elobbyist_do_not_flash_cache':
        if ($value) {
          $cache_unclear = TRUE;
        }
        break;
      case 'elobbyist_notify_body':
        $value_to_set = $value['value'];
        break;
      case 'elobbyist_sender_pass':
        if (empty($value))
          continue; //don't delete password when not entered
        else { 
          ////encrypt password
          $value_to_set = elobbyist_encrypt($value);
        }
        break;
      default:
        if (is_array($value)) {
          if (elobbyist_settings_form_set_values($form[$key], $values[$key])) {
            $cache_clear = TRUE;
          }
          continue;
        }
        //make sure only intended values are saved
        else if (strpos($key, 'elobbyist_') === 0) {
          $value_to_set = $value;
        }
        else {
          continue;
        }
        break;
    }

    if (elobbyist_settings_cache_clear_check($form, $key, $value_to_set )) {
      $cache_clear = TRUE;
    }

    variable_set($key, $value_to_set);
  }
  
  //user has chosen not to flush cache
  if ($cache_unclear) {
    return FALSE;
  }

  return $cache_clear;
}

function elobbyist_settings_cache_clear_check($form, $key, $value_to_set) {
  if (isset($form[$key]['#cache_invalidator'])) {
    if (isset($form[$key]['#default_value'])) {
      $default = variable_get($key, $form[$key]['#default_value']);
      //check if value has really changed before clearing cahce
      if ($default != $value_to_set) { 
        return TRUE;
      }
    }
    else {
      //cannot check if value has changed, so clear cache just in case
      return TRUE; 
    }
  }
  return FALSE;
}

/*
 * Restore functions
 */
function elobbyist_settings_form_restore_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  
  $form_state['redirect'] = array('admin/config/administration/elobbyist/restore',
    array('query' => $destination));
}

function elobbyist_settings_restore_confirm($form, &$form_state) {
  return confirm_form($form, 
    t('Are you sure you want to restore default settings for the module Email Lobbyist?'),
    'elobbyist',
    t('This action cannot be undone'),
    t('Restore'),
    t('Cancel')
  );
}

function elobbyist_settings_restore_confirm_submit($form, &$form_state) {

  elobbyist_settings_restore();
  
  drupal_set_message(t('The configuration options have been restored.'));
  
  $form_state['redirect'] = array('elobbyist/settings');
}

function elobbyist_config_records_per_page_options() {
  return array( 
      5 => 5,
      10 => 10,
      ELOBBYIST_DEFAULT_RECORDS_PER_PAGE => ELOBBYIST_DEFAULT_RECORDS_PER_PAGE,
      50 => 50,
      100 => 100,
      150 => 150,
      200 => 200,
      250 => 250,
      500 => 500,
  );
}
