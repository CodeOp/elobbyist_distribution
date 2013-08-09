<?php

/**
 * @file
 * Email provider entity pages callbacks
 */

/**
 * form prepare callback
 */
function elobbyist_email_provider_form($form, &$form_state, $entity) {
  // Set the id to identify this as a record edit form.
  $form['#id'] = 'elobbyist_email_provider-form';
  
  // Save the record for later, in case we need it.
  $form['#elobbyist_email_provider'] = $entity;
  $form_state['elobbyist_email_provider'] = $entity;
  
  // Set fields
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Provider Name'),
    '#default_value' => $entity->name,
    '#weight' => -5,
    '#maxlength' => 100,
    '#size' => 20,
    '#required' => TRUE,
  );
  
  $form['smtphost'] = array(
    '#type' => 'textfield',
    '#title' => t('SMTP Server Address'),
    '#default_value' => $entity->smtphost,
    '#weight' => 5,
    '#maxlength' => 255,
    '#size' => 40,
    '#required' => TRUE,
  );
    
  $form['smtpport'] = array(
    '#type' => 'textfield',
    '#title' => t('SMTP Server Port'),
    '#default_value' => $entity->smtpport,
    '#weight' => 20,
    '#maxlength' => 5,
    '#size' => 10,
    '#required' => TRUE,
  );
  
  //Add the buttons
  $form['buttons'] = array();
  $form['buttons']['#weight'] = 100;
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 100,
    '#submit' => array('elobbyist_email_provider_form_submit'),
  );
  
  $form['buttons']['save_and_add'] = array(
    '#type' => 'submit',
    '#value' => t('Save and Add'),
    '#weight' => 110,
    '#submit' => array('elobbyist_email_provider_form_submit', 'elobbyist_email_provider_redirect_to_add'),
  );
  
  if (!empty($entity->providerid)) {
    $form['buttons']['delete'] = array(
      '#access' => user_access('delete email lobbyist email providers'),
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 120,
      '#submit' => array('elobbyist_email_provider_form_delete_submit'),
      '#limit_validation_errors' => array(),       // No validation.
    );
  }
  
  $form['#validate'][] = 'elobbyist_email_provider_form_validate';
  
  field_attach_form('elobbyist_email_provider', $entity, $form, $form_state);
  
  return $form;
}

/**
 * Validate form
 */
function elobbyist_email_provider_form_validate($form, &$form_state) {
  $entity = $form_state['elobbyist_email_provider']; 
  
  //Field validation
  field_attach_form_validate('elobbyist_email_provider', $entity, $form, $form_state);
  
  if (!form_get_errors()) {
    //verify that name is unique
    elobbyist_unique_field('elobbyist_email_provider', $entity->providerid, 'providerid', 'name', 
      $form_state['values']['name'], t('Provider name'));
  }
}

/**
 * Save functions
 */
function elobbyist_email_provider_form_submit($form, &$form_state) {
  global $user;
  
  $entity = $form_state['elobbyist_email_provider'];
  
  $entity->name = $form_state['values']['name'];
  $entity->smtpport = $form_state['values']['smtpport'];
  $entity->smtphost = $form_state['values']['smtphost'];
    
  // Notify field widgets.
  field_attach_submit('elobbyist_email_provider', $entity, $form, $form_state);
  
  $new_record = empty($entity->providerid);
  
  // Save the record.
  $entity = elobbyist_email_provider_save($entity);
  
  // Notify the user.
  if ($entity) {
    drupal_set_message(t('Email provider saved.'));
    
    if ($new_record) {
      if (entity_access('update', 'elobbyist_email_provider')) {
        $form_state['redirect'] = 'elobbyist/email-provider/' . $entity->providerid . '/edit';
      }
      else {
        $form_state['redirect'] = 'elobbyist/email-providers/add';      
      }
    }
  }
}

function elobbyist_email_provider_save($entity) {
  return entity_get_controller('elobbyist_email_provider')->save($entity);
}

function elobbyist_email_provider_redirect_to_add($form, &$form_state) {
  $form_state['redirect'] = 'elobbyist/email-providers/add';
}

/*
 * Delete functions
 */
function elobbyist_email_provider_delete_confirm($form, &$form_state, $entity) {
  $form['#elobbyist_email_provider'] = $entity;
  
  $form['providerid'] = array('#type' => 'value', '#value' => $entity->providerid);
  return confirm_form($form, 
    t('Are you sure you want to delete %title?',
      array('%title' => $entity->name)),
    'elobbyist/email-providers',
    t('This action cannot be undone'),
    t('Delete'), 
    t('Cancel')
  );
}
function elobbyist_email_provider_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  
  $entity = $form['#elobbyist_email_provider'];
  $form_state['redirect'] = array('elobbyist/email-provider/' . $entity->providerid . '/delete',
    array('query' => $destination));
}

function elobbyist_email_provider_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $entity = elobbyist_email_provider_load($form_state['values']['providerid']);
    elobbyist_email_provider_delete($form_state['values']['providerid']);
    watchdog('elobbyist_email_provider', 'email provider deleted %title.' , 
      array('%title' => $entity->name) );
    
    drupal_set_message(t('Email provider %title has been deleted.', array(
      '%title' => $entity->name)
      )
    );
  }
  
  $form_state['redirect'] = 'elobbyist/email-providers';
}

/*
 * Add record page callback
 */
function elobbyist_email_provider_add() { 
  $entity = entity_get_controller('elobbyist_email_provider')->create();
  
  drupal_set_title(t('New Email Provider'), PASS_THROUGH);
  
  return drupal_get_form('elobbyist_email_provider_form', $entity);
}

/*
 * Edit record page callback
 */
function elobbyist_email_provider_page_edit($entity) {
  drupal_set_title(t('Email Provider: @title', array(
    '@title' => $entity->name, )), PASS_THROUGH);
  
  return drupal_get_form('elobbyist_email_provider_form', $entity);
}

