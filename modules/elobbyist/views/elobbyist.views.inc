<?php

/*
 * Implements hook_views_data()
 */
function elobbyist_views_data() {
  $data = array();
  $data['elobbyist_activity'] = array(
    'table' => array(
      'base' => array(
        'field' => 'aid',
        'title' => t('Elobbyist Activity'),
        'help' => t('Email Lobbyist email activity'),
        'weight' => 540,
       ),
      'group' => t('Elobbyist Activity'),
    ),
    'aid' => array(
      'title' => t('Activity id'),
      'help' => t('The primary identifier for an email activity.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'numeric' => TRUE,
        'validate type' => 'aid',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'mid' => array(
      'title' => t('Mailing id'),
      'help' => t('mailing id.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'relationship' => array(
          'title' => t('Mailing'),
          'lable' => t('Mailing'),
          'base' => 'elobbyist_mailing',
          'base field' => 'mid',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_many_to_one',
        'numeric' => TRUE,
      ),
      'filter' => array(
        'handler' => 'elobbyist_views_handler_filter_mailing',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'tid' => array(
      'title' => t('Template id'),
      'help' => t('template id.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'relationship' => array(
          'title' => t('Template'),
          'lable' => t('Template'),
          'base' => 'elobbyist_template',
          'base field' => 'tid',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_many_to_one',
        'numeric' => TRUE,
      ),
      'filter' => array(
        'handler' => 'elobbyist_views_handler_filter_template',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'fromcid' => array(
      'title' => t('From Contact id'),
      'help' => t('Contact id.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'relationship' => array(
          'title' => t('From'),
          'lable' => t('From'),
          'base' => 'elobbyist_contact',
          'base field' => 'cid',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_many_to_one',
        'numeric' => TRUE,
      ),
      'filter' => array(
        'handler' => 'elobbyist_views_handler_filter_contact',
        'contact type' => 'subscriber',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'tocid' => array(
      'title' => t('To Contact id'),
      'help' => t('Contact id.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'relationship' => array(
          'title' => t('To'),
          'lable' => t('To'),
          'base' => 'elobbyist_contact',
          'base field' => 'cid',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_many_to_one',
        'numeric' => TRUE,
      ),
      'filter' => array(
        'handler' => 'elobbyist_views_handler_filter_contact',
        'contact type' => 'recipient',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'created' => array(
      'title' => t('Created'),
      'help' => t('The Unix timestamp when the record was created.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
     ),
     'argument' => array(
       'handler' => 'views_handler_argument_date',
     ),
     'sort' => array(
       'handler' => 'views_handler_sort_date',
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_date',
     ),
    ),
    'view_link' => array(
      'real field' => 'aid',
      'title' => t('View'),
      'help' => t('A view link for the sent email'),
      'field' => array(
        'handler' => 'elobbyist_views_handler_link',
        'click sortable' => FALSE,
        'url' => 'elobbyist/activity/%',
        'link_text' => t('View'),
      ),
    ),
  );
  
  $lstContactFormTypes = elobbyist_form_types();
  $lstContactSubscriberTypes = elobbyist_subscriber_types();
  $lstContactEmailStatuses = elobbyist_email_statuses();
  
  $data['elobbyist_contact'] = array(
    'table' => array(
      'base' => array(
        'field' => 'cid',
        'title' => t('Elobbyist Contact'),
        'help' => t('Email Lobbyist contacts - subscribers and recipients'),
        'weight' => 500,
      ),
      'group' => t('Elobbyist Contact'),
    ),
    'cid' => array(
      'title' => t('Contact id'),
      'help' => t('The primary identifier for a contact.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'name field' => 'name', 
        'numeric' => TRUE,
        'validate type' => 'cid',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'edit_link' => array(
      'real field' => 'cid',
      'title' => t('Edit'),
      'help' => t('An edit link for the contact'),
      'field' => array(
        'handler' => 'elobbyist_views_handler_link',
        'click sortable' => FALSE,
        'url' => 'elobbyist/contact/%/edit',
        'link_text' => t('Edit'),
        'entity access' => 'update',
      ),
    ),
    'delete_link' => array(
      'real field' => 'cid',
      'title' => t('Delete'),
      'help' => t('A delete link for the contact'),
      'field' => array(
        'handler' => 'elobbyist_views_handler_link',
        'click sortable' => FALSE,
        'url' => 'elobbyist/contact/%/delete',
        'link_text' => t('Delete'),
        'entity access' => 'delete',
      ),
    ),
    'type' => array(
      'title' => t('Type'),
      'help' => t('The contact_type of this contact.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'name' => array(
      'title' => t('Name'),
      'help' => t('The contact full name.'),
      'field' => array(
        'handler' => 'elobbyist_views_handler_linked_name',
        'click sortable' => TRUE,
        'link_to_record default' => TRUE,
        'id field' => 'cid',
        'url' => 'elobbyist/contact/%/edit',
        'readonly url' => 'elobbyist/contact/%/view',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'email' => array(
      'title' => t('Email'),
      'help' => t('The contact email address.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'form' => array(
      'title' => t('Form'),
      'help' => t('for subscriber - creating form'),
      'field' => array(
        'handler' => 'elobbyist_views_handler_list_field',
        'list' => $lstContactFormTypes,
        'click sortable' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'numeric' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_in_operator',
        'options callback' => 'elobbyist_views_in_operator_callback',
        'options arguments' => array($lstContactFormTypes),
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    
    'stype' => array(
      'title' => t('Subscriber Type'),
      'help' => t('for subscriber - subscription type: persistent or specific-mailing subscriber?'),
      'field' => array(
        'handler' => 'elobbyist_views_handler_list_field',
        'list' => $lstContactSubscriberTypes,
        'click sortable' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'numeric' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_in_operator',
        'options callback' => 'elobbyist_views_in_operator_callback',
        'options arguments' => array($lstContactSubscriberTypes),
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'providerid' => array(
      'title' => t('Provider id'),
      'help' => t('The email provider identifier.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'relationship' => array(
          'title' => t('Email Provider'),
          'lable' => t('Email Provider'),
          'base' => 'elobbyist_email_provider',
          'base field' => 'providerid',
        ),
      'argument' => array(
        'handler' => 'views_handler_argument_many_to_one',
        'numeric' => TRUE,
      ),
      'filter' => array(
        'handler' => 'elobbyist_views_handler_filter_email_provider',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'notify' => array(
      'title' => t('Notify'),
      'help' => t('Notify on emails sent.'),
      'field' => array(
        'handler' => 'views_handler_field_boolean',
        'click sortable' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_boolean_operator',
        'label' => t('Notify on emails'),
        'type' => 'yes-no',
        'use equal' => TRUE, // Use status = 1 instead of status <> 0 in WHERE statment
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    
    'bounce' => array(
      'title' => t('Bounce'),
      'help' => t('bouncing status'),
      'field' => array(
        'handler' => 'elobbyist_views_handler_list_field',
        'list' => $lstContactEmailStatuses,
        'click sortable' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'numeric' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_in_operator',
        'options callback' => 'elobbyist_views_in_operator_callback',
        'options arguments' => array($lstContactEmailStatuses),
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'bounces' => array(
      'title' => t('Bounces'),
      'help' => t('bouncing count'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'numeric' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'created' => array(
      'title' => t('Created'),
      'help' => t('The Unix timestamp when the contact was created.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'argument' => array(
         'handler' => 'views_handler_argument_date',
       ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_date',
        ),
    ),
    'changed' => array(
      'title' => t('Changed'),
      'help' => t('The Unix timestamp when the contact was most recently saved.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'argument' => array(
         'handler' => 'views_handler_argument_date',
       ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
      ),
    ),
  );
  $data['elobbyist_email_provider'] = array(
    'table' => array(
      'base' => array(
        'field' => 'providerid',
        'title' => t('Elobbyist Email Provider'),
        'help' => t('Email Lobbyist email provider'),
        'weight' => 570,
      ),
      'group' => t('Elobbyist Email Provider'),
    ),
    'providerid' => array(
      'title' => t('Provider id'),
      'help' => t('The email provider identifier id.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'name field' => 'name', 
        'numeric' => TRUE,
        'validate type' => 'providerid',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'name' => array(
      'title' => t('Name'),
      'help' => t('The provider name.'),
      'field' => array(
        'handler' => 'elobbyist_views_handler_linked_name',
        'click sortable' => TRUE,
        'id field' => 'providerid',
        'link_to_record default' => TRUE,
        'url' => 'elobbyist/email-provider/%/edit',
        'readonly url' => 'elobbyist/email-provider/%/view',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'smtphost' => array(
      'title' => t('Smtp host'),
      'help' => t('The provider smtp server address.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'smtpport' => array(
      'title' => t('Smtp port'),
      'help' => t('The provider smtp server port'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'edit_link' => array(
      'real field' => 'providerid',
      'title' => t('Edit'),
      'help' => t('An edit page link'),
      'field' => array(
        'handler' => 'elobbyist_views_handler_link',
        'click sortable' => FALSE,
        'url' => 'elobbyist/email-provider/%/edit',
        'link_text' => t('Edit'),
        'entity access' => 'update',
      ),
    ),
    'delete_link' => array(
      'real field' => 'providerid',
      'title' => t('Delete'),
      'help' => t('A delete page link'),
      'field' => array(
        'handler' => 'elobbyist_views_handler_link',
        'click sortable' => FALSE,
        'url' => 'elobbyist/email-provider/%/delete',
        'link_text' => t('Delete'),
        'entity access' => 'delete',
      ),
    ),
  );
  
  $lstGroupStatuses = elobbyist_group_status_list();
  
  $data['elobbyist_group'] = array(
    'table' => array(
      'base' => array(
        'field' => 'grpid',
        'title' => t('Elobbyist Group'),
        'help' => t('Email Lobbyist contact group'),
        'weight' => 510,
      ),
      'group' => t('Elobbyist Group'),
    ),
    'grpid' => array(
      'title' => t('Group id'),
      'help' => t('The group id.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'name field' => 'name', 
        'numeric' => TRUE,
        'validate type' => 'grpid',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'name' => array(
      'title' => t('Name'),
      'help' => t('The group name.'),
      'field' => array(
        'handler' => 'elobbyist_views_handler_linked_name',
        'id field' => 'grpid',
        'click sortable' => TRUE,
        'link_to_record default' => TRUE,
        'url' => 'elobbyist/contact-group/%/edit',
        'readonly url' => 'elobbyist/contact-group/%/view',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'type' => array(
      'title' => t('Type'),
      'help' => t('The contact_type this group is confined to.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    
    'status' => array(
      'title' => t('Status'),
      'help' => t('group status: Active, Inactive.'),
      'field' => array(
        'handler' => 'elobbyist_views_handler_list_field',
        'list' => $lstGroupStatuses,
        'click sortable' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'numeric' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_in_operator',
        'options callback' => 'elobbyist_views_in_operator_callback',
        'options arguments' => array($lstGroupStatuses),
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'edit_link' => array(
      'real field' => 'grpid',
      'title' => t('Edit'),
      'help' => t('An edit page link'),
      'field' => array(
        'handler' => 'elobbyist_views_handler_link',
        'click sortable' => FALSE,
        'url' => 'elobbyist/contact-group/%/edit',
        'link_text' => t('Edit'),
        'entity access' => 'update',
      ),
    ),
    'delete_link' => array(
      'real field' => 'grpid',
      'title' => t('Delete'),
      'help' => t('A delete page link'),
      'field' => array(
        'handler' => 'elobbyist_views_handler_link',
        'click sortable' => FALSE,
        'url' => 'elobbyist/contact-group/%/delete',
        'link_text' => t('Delete'),
        'entity access' => 'delete',
      ),
    ),
    'members_link' => array(
      'real field' => 'grpid',
      'title' => t('Members'),
      'help' => t('Group member contacts'),
      'field' => array(
        'handler' => 'elobbyist_views_handler_link',
        'click sortable' => FALSE,
        'url' => 'elobbyist/contact-group/%/members',
        'link_text' => t('Members'),
        'entity access' => 'update',
      ),
    ),
  );
  
  //get array once for both field and filter handlers
  $lstMailingStatuses = elobbyist_mailing_status_list();
  $lstMailingScheduleTypes = elobbyist_mailing_schedule_type_list();
  
  $data['elobbyist_mailing'] = array(
    'table' => array(
      'base' => array(
        'field' => 'mid',
        'title' => t('Elobbyist Mailing'),
        'help' => t('Email Lobbyist mailing'),
        'weight' => 520,
      ),
      'group' => t('Elobbyist Mailing'),
    ),
    'mid' => array(
      'title' => t('Mailing id'),
      'help' => t('The primary identifier for a mailing.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'name field' => 'name', 
        'numeric' => TRUE,
        'validate type' => 'mid',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'name' => array(
      'title' => t('Name'),
      'help' => t('The mailing name.'),
      'field' => array(
        'handler' => 'elobbyist_views_handler_linked_name',
        'id field' => 'mid',
        'click sortable' => TRUE,
        'link_to_record default' => TRUE,
        'url' => 'elobbyist/mailing/%/edit',
        'readonly url' => 'elobbyist/mailing/%/view',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'tid' => array(
      'title' => t('Template id'),
      'help' => t('The mailing template.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'relationship' => array(
          'title' => t('Template'),
          'lable' => t('Template'),
          'base' => 'elobbyist_template',
          'base field' => 'tid',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_many_to_one',
        'numeric' => TRUE,
      ),
      'filter' => array(
        'handler' => 'elobbyist_views_handler_filter_template',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'schedule' => array(
      'title' => t('Schedule'),
      'help' => t('schedule type.'),
      'field' => array(
        'handler' => 'elobbyist_views_handler_list_field',
        'list' => $lstMailingScheduleTypes,
        'click sortable' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'numeric' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_in_operator',
        'options callback' => 'elobbyist_views_in_operator_callback',
        'options arguments' => array($lstMailingScheduleTypes),
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'status' => array(
      'title' => t('Status'),
      'help' => t('mailing status: Draft, Scheduled, Cancelled, Closed.'),
      'field' => array(
        'handler' => 'elobbyist_views_handler_list_field',
        'list' => $lstMailingStatuses,
        'click sortable' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'numeric' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_in_operator',
        'options callback' => 'elobbyist_views_in_operator_callback',
        'options arguments' => array($lstMailingStatuses),
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'page' => array(
      'title' => t('Page'),
      'help' => t('content of the call to action page'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => FALSE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    ),
    'successurl' => array(
      'title' => t('Success URL'),
      'help' => t('URL to redirect to after a successful public subscription'),
      'field' => array(
        'handler' => 'views_handler_field_url',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'start' => array(
      'title' => t('Start'),
      'help' => t('The Unix timestamp for scheduling start time.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'argument' => array(
         'handler' => 'views_handler_argument_date',
       ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
      ),
    ),
    'end' => array(
      'title' => t('End'),
      'help' => t('The Unix timestamp for scheduling end time.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'argument' => array(
         'handler' => 'views_handler_argument_date',
       ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
      ),
    ),
    'addtogrpid' => array(
      'title' => t('Collector Group id'),
      'help' => t('Collects subscribers of on-going schedule type'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => FALSE,
      ),
      'relationship' => array(
          'title' => t('Collector Group'),
          'lable' => t('Collector Group'),
          'base' => 'elobbyist_group',
          'base field' => 'grpid',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_many_to_one',
        'numeric' => TRUE,
      ),
      'filter' => array(
        'handler' => 'elobbyist_views_handler_filter_group',
        'group type' => 'subscribers',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'created' => array(
      'title' => t('Created'),
      'help' => t('The Unix timestamp when the record was created.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'argument' => array(
         'handler' => 'views_handler_argument_date',
       ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_date',
        ),
    ),
    'changed' => array(
      'title' => t('Changed'),
      'help' => t('The Unix timestamp when the record was most recently saved.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'argument' => array(
         'handler' => 'views_handler_argument_date',
       ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
      ),
    ),
    'edit_link' => array(
      'real field' => 'mid',
      'title' => t('Edit'),
      'help' => t('An edit page link'),
      'field' => array(
        'handler' => 'elobbyist_views_handler_link',
        'click sortable' => FALSE,
        'url' => 'elobbyist/mailing/%/edit',
        'link_text' => t('Edit'),
        'entity access' => 'update',
      ),
    ),
    'delete_link' => array(
      'real field' => 'mid',
      'title' => t('Delete'),
      'help' => t('A delete page link'),
      'field' => array(
        'handler' => 'elobbyist_views_handler_link',
        'click sortable' => FALSE,
        'url' => 'elobbyist/mailing/%/delete',
        'link_text' => t('Delete'),
        'entity access' => 'delete',
      ),
    ),
  );
  
  $lstTemplateStatuses = elobbyist_template_status_list();
  
  $data['elobbyist_template'] = array(
    'table' => array(
      'base' => array(
        'field' => 'tid',
        'title' => t('Elobbyist Template'),
        'help' => t('Email Lobbyist template'),
        'weight' => 560,
      ),
      'group' => t('Elobbyist Template'),
    ),
    'tid' => array(
      'title' => t('Template id'),
      'help' => t('The primary identifier for a template.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'name field' => 'name', 
        'numeric' => TRUE,
        'validate type' => 'tid',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'name' => array(
      'title' => t('Name'),
      'help' => t('The template name.'),
      'field' => array(
        'handler' => 'elobbyist_views_handler_linked_name',
        'id field' => 'tid',
        'click sortable' => TRUE,
        'link_to_record default' => TRUE,
        'url' => 'elobbyist/template/%/edit',
        'readonly url' => 'elobbyist/template/%/view',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'subject' => array(
      'title' => t('Subject'),
      'help' => t('The email subject.'),
      'field' => array(
        'handler' => 'elobbyist_views_handler_linked_name',
        'id field' => 'tid',
        'click sortable' => TRUE,
        'link_to_record default' => TRUE,
        'url' => 'elobbyist/template/%/edit',
        'readonly url' => 'elobbyist/template/%/view',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'body' => array(
      'title' => t('Body'),
      'help' => t('The email body.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => FALSE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    ),
    'status' => array(
      'title' => t('Status'),
      'help' => 'template status: Draft, Sent, Closed, Cancelled.',
      'field' => array(
        'handler' => 'elobbyist_views_handler_list_field',
        'list' => $lstTemplateStatuses,
        'click sortable' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'numeric' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_in_operator',
        'options callback' => 'elobbyist_views_in_operator_callback',
        'options arguments' => array($lstTemplateStatuses),
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'edit_link' => array(
      'real field' => 'tid',
      'title' => t('Edit'),
      'help' => 'An edit page link',
      'field' => array(
        'handler' => 'elobbyist_views_handler_link',
        'click sortable' => FALSE,
        'url' => 'elobbyist/template/%/edit',
        'link_text' => t('Edit'),
        'entity access' => 'update',
      ),
    ),
    'delete_link' => array(
      'real field' => 'tid',
      'title' => t('Delete'),
      'help' => 'A delete page link',
      'field' => array(
        'handler' => 'elobbyist_views_handler_link',
        'click sortable' => FALSE,
        'url' => 'elobbyist/template/%/delete',
        'link_text' => t('Delete'),
        'entity access' => 'delete',
      ),
    ),
  );

  return $data;
}


function elobbyist_views_in_operator_callback($vals) {
  return $vals;
}