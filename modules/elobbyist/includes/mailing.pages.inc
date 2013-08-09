<?php

/**
 * @file
 * Mailing entity pages callbacks
 */

/**
 * form prepare callback
 */
function elobbyist_mailing_form($form, &$form_state, $entity) {
  // Set the id to identify this as a record edit form.
  $form['#id'] = 'elobbyist_mailing-form';
  
  drupal_add_css(drupal_get_path('module', 'elobbyist') . 
    '/includes/elobbyist.css', array('type' => 'file'));

  // Save the record for later, in case we need it.
  $form['#elobbyist_mailing'] = $entity;
  $form_state['elobbyist_mailing'] = $entity;
  
  drupal_add_js(array('OnGoingSchedule' => ELOBBYIST_MAILING_SCHEDULE_ON_GOING), 'setting');
  
  elobbyist_add_js_file('mailing.js');

  // Set fields
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Mailing Name'),
    '#default_value' => $entity->name,
    '#weight' => -5,
    '#maxlength' => 100,
    '#size' => 83,
    '#required' => TRUE,
    '#description' => t('This is also the title of the public call to action page, if exists'),
  );

  $form['tid'] = array(
      '#type' => 'select',
      '#key_type' => 'associative',
      '#title' => t('Email Template'),
      '#options' => array('' => '') + elobbyist_get_templates($entity->tid),
      '#default_value' => $entity->tid,
      '#weight' => 0,
      '#required' => TRUE,
    );

  $form['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#default_value' => $entity->status,
    '#weight' => 5,
    '#options' => elobbyist_mailing_status_list(),
    '#required' => TRUE,
  );
  
  $form['elobbyist_from'] = array(
    '#type' => 'elobbyist_from',
    '#relation_type' => 'elobbyist_from',
    '#title' => t('From'),
    '#weight' => 6,
    '#description' => t('Required for a simple run mailing. Optional for an on-going one.'),
  );

  $form['elobbyist_to'] = array(
      '#type' => 'elobbyist_to',
      '#relation_type' => 'elobbyist_to',
      '#title' => t('To'),
      '#weight' => 7,
  );
  
  $form['start'] = array(
    '#type' => 'date_popup',
    '#date_type' => DATE_UNIX,
    '#date_format' => ELOBBYIST_DATE_FULL_FORMAT,
    '#title' => t('Start Date'),
    '#default_value' => format_date($entity->start, 'custom', ELOBBYIST_DATE_FULL_FORMAT),
    '#date_year_range' => '-0:+' . ELOBBYIST_MAILING_SCHEDULE_MAX_YEARS,
    '#weight' => 8,
    '#required' => FALSE,
    '#exclude_cv' => TRUE,
  );
  
  

  $form['schedule'] = array(
    '#type' => 'select',
    '#title' => t('Schedule type'),
    '#default_value' => $entity->schedule,
    '#weight' => 10,
    '#options' => elobbyist_mailing_schedule_type_list(),
    '#required' => TRUE,
    '#attributes' => array(
      'onchange' => 'JavaScript:OnScheduleChange();'
      )
  );
  
  $cur_schedule = $entity->schedule;
  if (isset($form_state['input']['schedule'])) {
    $cur_schedule = $form_state['input']['schedule'];
  }
  
  $form['ongoing'] = array(
    '#type' => 'fieldset',
    '#title' => t('On going schedule'),
    '#weight' => 11,
  );
  
  if ($cur_schedule == ELOBBYIST_MAILING_SCHEDULE_ON_GOING) {
    $form['ongoing']['#attributes']['style'][] = 'display:block;';
  }
  else {
    $form['ongoing']['#attributes']['style'][] = 'display:none;';
  }
  
  if (!empty($entity->mid)) {
    $form['ongoing']['link'] = array(
      '#markup' => theme('elobbyist_html',
              array(
                'title' => t('Public call to action link'),
                'item' => l(filter_xss('elobbyist/' . $entity->mid), 'elobbyist/' . $entity->mid,
                  array('attributes' => array('target' => '_blank'))),
              )
           ),
      '#weight' => 12,
    );
  }
  
  $form['ongoing']['addtogrpid'] = array(
    '#type' => 'select',
    '#title' => t('Subscribers Group'),
    '#default_value' => $entity->addtogrpid,
    '#weight' => 13,
    '#options' =>  array('' => '') + elobbyist_get_groups('subscribers'),
    '#required' => FALSE, 
    '#description' => t('Required. New subscribers from the public call to action page will be added to this group. Note: setting this field to a group included in the From field works but affects badly on performance.'),
  );
  
  $tmp = '';
  if (!empty($entity->end)) {
    $tmp = format_date($entity->end, 'custom', ELOBBYIST_DATE_FULL_FORMAT);
  }
  
  $form['ongoing']['end'] = array(
    '#type' => 'date_popup',
    '#date_type' => DATE_UNIX,
    '#date_format' => ELOBBYIST_DATE_FULL_FORMAT,
    '#title' => t('End Date'),
    '#default_value' => $tmp,
    '#weight' => 14,
    '#required' => FALSE,
    '#exclude_cv' => TRUE,
    '#attributes' => array(
      'onchange' => 'JavaScript:OnEndDateChange();'
      ),
  );
  
  $form['ongoing']['successurl'] = array(
    '#type' => 'textfield',
    '#title' => t('Success URL'),
    '#default_value' => $entity->successurl,
    '#weight' => 15,
    '#required' => FALSE,
    '#description' => t('Redirect to this url on successful public subscription'),
  );
  
  //Add the buttons
  $form['buttons'] = array();
  $form['buttons']['#weight'] = 100;
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 100,
    '#submit' => array('elobbyist_mailing_form_submit'),
  );
  
  $form['buttons']['save_and_add'] = array(
    '#type' => 'submit',
    '#value' => t('Save and Add'),
    '#weight' => 110,
    '#submit' => array('elobbyist_mailing_form_submit', 'elobbyist_mailing_redirect_to_add'),
  );
  
  if (!empty($entity->mid)) {
    $form['buttons']['delete'] = array(
      '#access' => user_access('delete email lobbyist mailings'),
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 120,
      '#submit' => array('elobbyist_mailing_form_delete_submit'),
      '#limit_validation_errors' => array(),       // No validation.
    );
  }
  
  $form['#validate'][] = 'elobbyist_mailing_form_validate';
  
  field_attach_form('elobbyist_mailing', $entity, $form, $form_state);
  
  return $form;
}

function elobbyist_mailing_page_form($form, &$form_state, $entity) {
  // Set the id to identify this as a record edit form.
  $form['#id'] = 'elobbyist_mailing-page-form';

  // Save the record for later, in case we need it.
  $form['#elobbyist_mailing'] = $entity;
  $form_state['elobbyist_mailing'] = $entity; 
    
  $form['link'] = array(
    '#markup' => theme('elobbyist_html',
            array(
              'title' => t('Link'),
              'item' => l(filter_xss('elobbyist/' . $entity->mid), 'elobbyist/' . $entity->mid,
                array('attributes' => array('target' => '_blank'))),
            )
         ),
    '#weight' => -50,
  );
  
  $form['page'] = array(
    '#type' => 'text_format',
    '#title' => t('Page content'),
    '#default_value' => $entity->page,
    '#weight' => 20,
    '#required' => TRUE,
  );
  
  //Add the buttons
  $form['buttons'] = array();
  $form['buttons']['#weight'] = 100;
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 100,
    '#submit' => array('elobbyist_mailing_page_form_submit'),
  );
  
  return $form;
}


/**
 * Validate form
 */
function elobbyist_mailing_form_validate($form, &$form_state) {
  
  $entity = $form_state['elobbyist_mailing'];
  
  $entity->name = $form_state['values']['name'];
  $entity->tid = $form_state['values']['tid'];
  $entity->schedule = $form_state['values']['schedule'];
  $entity->status = $form_state['values']['status'];  

  if (!empty($form_state['values']['start'])) {
    $entity->start = elobbyist_string_to_full_date($form_state['values']['start']);
  }
  
  //for manual task always set end date to start date
  if ($entity->schedule == ELOBBYIST_MAILING_SCHEDULE_SIMPLE) {
    $entity->end = $entity->start;
  }
  else { //on going task   
    $entity->successurl = $form_state['values']['successurl'];
    $entity->addtogrpid = $form_state['values']['addtogrpid'];
    
    if (empty($entity->addtogrpid)) {
      form_set_error('addtogrpid', t('An on going schedule type requires filling Subscribers Group.'));
    }
    
    if (!empty($form_state['values']['end']) && !is_array($form_state['values']['end'])) {
      $entity->end = elobbyist_string_to_full_date($form_state['values']['end']);

      if ($entity->end < ($entity->start)) {
        form_set_error('end', t('End date must be greater than or equal to Start date'));
      }
    }
    else {
      form_set_error('end', t('End date must be set for an on-going schedule type.'));
    }
  }
  
  $form_state['elobbyist_mailing'] = $entity;
  
  //do not allow saving as active if to and from are not filled
  if ($entity->status == ELOBBYIST_MAILING_STATUS_ACTIVE) {
    if ($entity->schedule == ELOBBYIST_MAILING_SCHEDULE_SIMPLE) {
      if (!elobbyist_groups_field_has_value('elobbyist_from', 'elobbyist_mailing', $form, $form_state)) {
        form_set_error('elobbyist_from', t('An active mailing requires filling senders (From).'));
      }
    }
    if (!elobbyist_groups_field_has_value('elobbyist_to', 'elobbyist_mailing', $form, $form_state)) {
      form_set_error('elobbyist_to', t('An active mailing requires filling recipients (To).'));
    }
  }
  
  //Field validation
  field_attach_form_validate('elobbyist_mailing', $entity, $form, $form_state);
  
  if (!form_get_errors()) {
    //verify that name is unique
    elobbyist_unique_field('elobbyist_mailing', $entity->mid, 'mid', 'name', 
      $form_state['values']['name'], t('Mailing name'));
  }
}


/**
 * Save functions
 */
function elobbyist_mailing_form_submit($form, &$form_state) {
  global $user;
  
  $entity = $form_state['elobbyist_mailing'];
  
  // Notify field widgets.
  field_attach_submit('elobbyist_mailing', $entity, $form, $form_state);
  
  $new_record = empty($entity->mid);
  
  // Save the record.
  $entity = elobbyist_mailing_save($entity);
  
  //if new record, call submit from and to manually, because it needs the entity id
  if ($new_record) {
    elobbyist_field_attach_submit('elobbyist_mailing', $entity, $form, $form_state);
  }

  
  // Notify the user.
  if ($entity) {
    drupal_set_message(t('Mailing saved.'));
    
    if ($entity->status == ELOBBYIST_MAILING_STATUS_ACTIVE &&
        $entity->schedule == ELOBBYIST_MAILING_SCHEDULE_ON_GOING &&
        $entity->end < REQUEST_TIME) {
      drupal_set_message(t('As long as this mailing dates are in the past it\'s public call to action page won\'t be open to new subscribers.'), 'warning');
    }
    
    if ($new_record) {
      if (entity_access('update', 'elobbyist_mailing')) {
        $form_state['redirect'] = 'elobbyist/mailing/' . $entity->mid . '/edit';
      }
      elseif (entity_access('view', 'elobbyist_mailing')) {
        $form_state['redirect'] = 'elobbyist/mailing/' . $entity->mid . '/view';
      }
      else {
        $form_state['redirect'] = 'elobbyist/mailings/add';
      }
    }
  }
}

function elobbyist_mailing_page_form_submit($form, &$form_state) {
  
  $entity = $form_state['elobbyist_mailing'];
  
  $entity->page = $form_state['values']['page']['value'];
  
  $entity = elobbyist_mailing_save($entity);
  
  // Notify the user.
  if ($entity) {
    if ($entity->status == ELOBBYIST_MAILING_STATUS_ACTIVE) {
      drupal_set_message(
        t('Public call to action was saved. The page is published.') );
    }
    elseif ($entity->status == ELOBBYIST_MAILING_STATUS_DRAFT) {
      drupal_set_message(
        t('Public call to action was saved. Notice @pagelink is only accessible to authenticated users, as long as the mailing is in draft status',
          array('@pagelink' => l(t('the page'), 'elobbyist/' . $entity->mid))));
    }
  }
}


function elobbyist_mailing_save($entity) {
  return entity_get_controller('elobbyist_mailing')->save($entity);
}

function elobbyist_mailing_redirect_to_add($form, &$form_state) {
  $form_state['redirect'] = 'elobbyist/mailings/add';
}

/*
 * Delete functions
 */
function elobbyist_mailing_delete_confirm($form, &$form_state, $entity) {
  $form['#elobbyist_mailing'] = $entity;
  
  $form['mid'] = array('#type' => 'value', '#value' => $entity->mid);
  return confirm_form($form, 
    t('Are you sure you want to delete %title?',
      array('%title' => $entity->name)),
    'elobbyist/mailings',
    t('This action cannot be undone'),
    t('Delete'), 
    t('Cancel')
  );
}
function elobbyist_mailing_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  
  $entity = $form['#elobbyist_mailing'];
  $form_state['redirect'] = array('elobbyist/mailing/' . $entity->mid . '/delete',
    array('query' => $destination));
}

function elobbyist_mailing_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $entity = elobbyist_mailing_load($form_state['values']['mid']);
    elobbyist_mailing_delete($form_state['values']['mid']);
    watchdog('elobbyist_mailing', 'mailing deleted %title.' , 
      array('%title' => $entity->name) );
    
    drupal_set_message(t('Mailing %title has been deleted.', array(
      '%title' => $entity->name)
      )
    );
  }
  
  $form_state['redirect'] = 'elobbyist/mailings';
}

function elobbyist_mailing_page_title($entity) {
  drupal_set_title($entity->name, PASS_THROUGH);
}

function elobbyist_mailing_page_view($entity, $view_mode = 'full') {
  elobbyist_mailing_page_title($entity);
  
  // Remove previously built content, if exists.
  $entity->content = array();
  
  $entity->content['#attached']['css'] = array(
    drupal_get_path('module', 'elobbyist') . '/includes/elobbyist.css',
  );

  if (!empty($entity->tid)) {
    $tmp = entity_load('elobbyist_template', array($entity->tid));
    
    $sTemplateLink = NULL;
    if (entity_access('update', 'elobbyist_template')) {
      $sTemplateLink = l(filter_xss(current($tmp)->name), 'elobbyist/template/' . $entity->tid . '/edit');
    }
    elseif (entity_access('view', 'elobbyist_template')) {
      $sTemplateLink = l(filter_xss(current($tmp)->name), 'elobbyist/template/' . $entity->tid . '/view');
    }
    else {
      $sTemplateLink = filter_xss(current($tmp)->name);
    }
    
      
    if ($tmp != null && is_array($tmp)) {
      $entity->content['tid'] = array(
        '#markup' => theme('elobbyist_html',
            array(
              'title' => t('Template'),
              'item' => $sTemplateLink,
            )
         ),
        '#weight' => -45,
      );
    }
  }
  
  $schedules = elobbyist_mailing_schedule_type_list();
  $entity->content['schedule'] = array(
      '#markup' => theme('elobbyist_field',
          array(
            'title' => t('Schedule'),
            'item' => $schedules[$entity->schedule],
          )
       ),
      '#weight' => -42,
    );

  if (!empty($entity->status)) {
    $statuses = elobbyist_mailing_status_list();
    $entity->content['status'] = array(
      '#markup' => theme('elobbyist_field',
          array(
            'title' => t('Status'),
            'item' => $statuses[$entity->status],
          )
       ),
      '#weight' => -40,
    );
  }
  
  $entity->content['start'] = array(
    '#markup' => theme('elobbyist_field',
          array(
            'title' => t('Start Date'),
            'item' => format_date($entity->start, 'custom', ELOBBYIST_DATE_FULL_FORMAT),
          )
       ),
      '#weight' => -37,
  );
  
  if  ($entity->schedule == ELOBBYIST_MAILING_SCHEDULE_ON_GOING) {
    $entity->content['link'] = array(
      '#markup' => theme('elobbyist_html',
              array(
                'title' => t('Public call to action link'),
                'item' => l(filter_xss('elobbyist/' . $entity->mid), 'elobbyist/' . $entity->mid,
                  array('attributes' => array('target' => '_blank'))),
              )
           ),
      '#weight' => -30,
    );
    
    $entity->content['end'] = array(
      '#markup' => theme('elobbyist_field',
          array(
            'title' => t('End Date'),
            'item' => format_date($entity->end, 'custom', ELOBBYIST_DATE_FULL_FORMAT),
          )
       ),
      '#weight' => -36,
    );
    
    if (!empty($entity->addtogrpid)) {
      $tmp = entity_load('elobbyist_group', array($entity->addtogrpid));

      if ($tmp != null && is_array($tmp)) {
        $entity->content['addtogrpid'] = array(
          '#markup' => theme('elobbyist_html',
              array(
                'title' => t('Subscribers Group'),
                'item' => l(filter_xss(current($tmp)->name), 'elobbyist/contact-group/' . $entity->addtogrpid . '/view'),
              )
           ),
          '#weight' => -20,
        );
      }
    }
    
    if (!empty($entity->successurl)) {
      $url = check_url($entity->successurl);

      $entity->content['successurl'] = array(
        '#markup' => theme('elobbyist_html',
              array(
                'title' => t('Success URL'),
                'item' => l($url, $url),
              )
           ),
          '#weight' => -15,
      ); 
    }
  }
  
  $entity->content['created'] = array(
    '#markup' => theme('elobbyist_field',
          array(
            'title' => t('Created on'),
            'item' => format_date($entity->created, 'custom', ELOBBYIST_DATE_FULL_FORMAT),
          )
       ),
      '#weight' => -2,
  );
  
  $entity->content['changed'] = array(
    '#markup' => theme('elobbyist_field',
          array(
            'title' => t('Modified on'),
            'item' => format_date($entity->changed, 'custom', ELOBBYIST_DATE_FULL_FORMAT),
          )
       ),
      '#weight' => -1,
  );
  
  // Build fields content.
  field_attach_prepare_view('elobbyist_mailing', 
    array($entity->mid => $entity), $view_mode);
  
  entity_prepare_view('elobbyist_mailing', array($entity->mid => $entity));
  
  $entity->content += field_attach_view('elobbyist_mailing', $entity, $view_mode);
  
  return $entity->content;
}

function elobbyist_mailing_sent_emails_page($entity, $view_mode = 'full') {
  drupal_set_title(t('@mailing\'s Emails', array('@mailing' => $entity->name)));
 
  $view = views_get_view('elobbyist_activities_by_mailing');

  return $view->preview('default', array($entity->mid));
}

/*
 * Edit the mailing page field - only if mailing is of schedule type 'on-going' and status active or draft
 * On simple run mailing: display message. On closed/cancelled: show the content without editing
 */
function elobbyist_mailing_action_page_edit($entity) {
  elobbyist_mailing_page_title($entity);
  
  if ($entity->schedule != ELOBBYIST_MAILING_SCHEDULE_ON_GOING) {
    drupal_set_message(t('This section is only relevant for an on-going schedule type.'), 'warning');
    return array();
  }
    
  switch($entity->status) {
    case ELOBBYIST_MAILING_STATUS_ACTIVE:
      if ($entity->end < REQUEST_TIME) {
        drupal_set_message(t('The public call to action page can be edited, but it is not published because the mailing end date has past.'), 'warning');
      }
      elseif ($entity->start >= REQUEST_TIME) {
        drupal_set_message(t('The public call to action page is published.'), 'status');
      }
      break;
    case ELOBBYIST_MAILING_STATUS_DRAFT:
      if ($entity->end < REQUEST_TIME) {
        drupal_set_message(t('The public call to action page can be edited, but it is not published because the mailing end date has past.'), 'warning');
      }
      else {
        drupal_set_message(t('The public call to action page can be edited, but it is not published because the mailing is in draft status.'), 'warning');
      }
      break;
    default:
      //if the page isn't in status draft or active, it should be read-only
      $entity->content = array();
      $entity->content['page'] = array(
        '#markup' => $entity->page,
      );
      drupal_set_message(t('This section is read-only and the public call to action page is not published when the mailing is in closed or cancelled status'), 'warning');
      return $entity->content;
  }
  
  return drupal_get_form('elobbyist_mailing_page_form', $entity);
}

/*
 * Add record page callback
 */
function elobbyist_mailing_add() { 
  $entity = entity_get_controller('elobbyist_mailing')->create();
  
  drupal_set_title(t('New Mailing'), PASS_THROUGH);
  
  return drupal_get_form('elobbyist_mailing_form', $entity);
}

/*
 * Edit record page callback
 */
function elobbyist_mailing_page_edit($entity) {
  elobbyist_mailing_page_title($entity);
  
  return drupal_get_form('elobbyist_mailing_form', $entity);
}


/*
 * Pull a list of templates from the database
 */
function elobbyist_get_templates($current) {
  $result = db_select('elobbyist_template', 't')
          ->fields('t', array('tid', 'name'))
          ->condition('status', ELOBBYIST_TEMPLATE_STATUS_ACTIVE)
          ->orderBy('t.name', 'ASC')
          ->execute();

  if (!empty($current))
  {
    $arr = $result->fetchAllKeyed();
    //if current option is not in list, add it
    if (!isset($arr[$current])) {
      $tmp = entity_load('elobbyist_template', array($current));
      if ($tmp != null) {
        $t_obj = current($tmp);
        $arr[$t_obj->tid] = $t_obj->name;
      }
    }
    
    return $arr;
  }
  
  return $result->fetchAllKeyed();
}