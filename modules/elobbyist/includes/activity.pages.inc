<?php


function elobbyist_activity_page_view($entity, $view_mode = 'full') {
  //load additional data
  $template = current(entity_load('elobbyist_template', array($entity->tid)));
  $contacts = entity_load('elobbyist_contact', array($entity->fromcid, $entity->tocid));
  $from = $contacts[$entity->fromcid];
  $to = $contacts[$entity->tocid];
  
  //set title
  drupal_set_title(
      elobbyist_template_to_email_subject($template->subject,
        $from->name, $to->name), 
    PASS_THROUGH);
  
  //show content
  $entity->content = array();
  
  $entity->content['#attached']['css'] = array(
    drupal_get_path('module', 'elobbyist') . '/includes/elobbyist.css',
  );
  
  $entity->content['sent'] = array(
    '#markup' => theme('elobbyist_field',
          array(
            'title' => t('Sent'),
            'item' => format_date($entity->created, 'custom', ELOBBYIST_DATE_FULL_FORMAT),
          )
       ),
      '#weight' => -50,
  );
  
  $sFromItem = NULL;
  $sToItem = NULL;
  
  if (entity_access('update', 'elobbyist_contact')) {
    $sFromItem = l(filter_xss_admin($from->name), 'elobbyist/contact/' . $entity->fromcid .  '/edit') . ' [' . $from->email . ']';
    $sToItem = l(filter_xss_admin($to->name), 'elobbyist/contact/' . $entity->tocid .  '/edit') . ' [' . $to->email . ']';
  }
  elseif (entity_access('view', 'elobbyist_contact')) {
    $sFromItem = l(filter_xss_admin($from->name), 'elobbyist/contact/' . $entity->fromcid .  '/view') . ' [' . $from->email . ']';
    $sToItem = l(filter_xss_admin($to->name), 'elobbyist/contact/' . $entity->tocid .  '/view') . ' [' . $to->email . ']';
  }
  else {
    $sFromItem =filter_xss_admin($from->name) . ' [' . $from->email . ']';
    $sToItem = filter_xss_admin($to->name) . ' [' . $to->email . ']';
  }
  
  $entity->content['from'] = array(
    '#markup' => 
      theme('elobbyist_html',
        array(
          'title' => t('From'),
          'item' => $sFromItem,
        )
     ),
    '#weight' => -40,
  );

  $entity->content['to'] = array(
    '#markup' => 
      theme('elobbyist_html',
        array(
          'title' => t('To'),
          'item' => $sToItem,
        )
     ),
    '#weight' => -30,
  );
  
  $entity->content['content'] = array(
    '#markup' => 
      theme('elobbyist_html',
        array(
          'title' => t('Message body'),
          'item' => 
            elobbyist_template_to_email_body($template->body, 
              $from->name, $to->name),
        )
     ),
    '#weight' => -20,
  );
  
  if (entity_access('update', 'elobbyist_mailing')) {
    $entity->content['mailinglink'] = array(
      '#markup' => 
          '<p>' . l(t('Mailing'), 'elobbyist/mailing/' . $entity->mid . '/edit') . '</p>',
      '#weight' => -10,
    );
  }
  elseif ( entity_access('view', 'elobbyist_mailing') ) {
    $entity->content['mailinglink'] = array(
      '#markup' => 
          '<p>' . l(t('Mailing'), 'elobbyist/mailing/' . $entity->mid . '/view') . '</p>'
       ,
      '#weight' => -10,
    );
  }
  
  field_attach_prepare_view('elobbyist_activity', 
    array($entity->aid => $entity), $view_mode);
  
  entity_prepare_view('elobbyist_activity', array($entity->aid => $entity));
  
  $entity->content += field_attach_view('elobbyist_activity', $entity, $view_mode);
    
  return $entity->content;
}