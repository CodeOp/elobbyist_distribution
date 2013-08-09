<?php

/*
 * Implements hook_views_default_views
 */
function elobbyist_views_default_views() {
  $views = array();
  
  $items_per_page = variable_get('elobbyist_records_per_page', ELOBBYIST_DEFAULT_RECORDS_PER_PAGE);
  
  //-----------------------
  //----------------------- Subscribers
  //-----------------------
  
  $view = new view();
  $view->name = 'elobbyist_subscribers';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'elobbyist_contact';
  $view->human_name = t('Email Lobbyist Subscribers');
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view email lobbyist contacts';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = t('Go');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = $items_per_page;
  
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'email' => 'email',
    'bounce' => 'bounce',
    'stype' => 'stype',
    'notify' => 'notify',
    'edit_link' => 'edit_link',
    'provider_name' => 'provider_name',
  );
  $handler->display->display_options['style_options']['default'] = 'name';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'email' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'bounce' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'stype' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_link' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'provider_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'notify' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  
  
  /* Field: Bulk operations: Elobbyist Contact */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => t('Delete'),
    ),
    'rules_component::elobbyist_add_contacts_to_group' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 0,
      'label' => '',
    ),
  );
  
  /* Relationship: Elobbyist Contact: Email Provider */
  $handler->display->display_options['relationships']['providerid']['id'] = 'providerid';
  $handler->display->display_options['relationships']['providerid']['table'] = 'elobbyist_contact';
  $handler->display->display_options['relationships']['providerid']['field'] = 'providerid';
  /* Field: Elobbyist Contact: Contact Id */
  $handler->display->display_options['fields']['cid']['id'] = 'cid';
  $handler->display->display_options['fields']['cid']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['cid']['field'] = 'cid';
  $handler->display->display_options['fields']['cid']['exclude'] = TRUE;
  
  /* Field: Elobbyist Contact: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = t('Full Name');
  /* Field: Elobbyist Contact: Email */
  $handler->display->display_options['fields']['email']['id'] = 'email';
  $handler->display->display_options['fields']['email']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['email']['field'] = 'email';
  
  /* Field: Elobbyist Email Provider: Provider Id */
  $handler->display->display_options['fields']['providerid']['id'] = 'providerid';
  $handler->display->display_options['fields']['providerid']['table'] = 'elobbyist_email_provider';
  $handler->display->display_options['fields']['providerid']['field'] = 'providerid';
  $handler->display->display_options['fields']['providerid']['relationship'] = 'providerid';
  $handler->display->display_options['fields']['providerid']['exclude'] = TRUE;
  
  /* Field: Elobbyist Email Provider: Name */
  $handler->display->display_options['fields']['provider_name']['id'] = 'provider_name';
  $handler->display->display_options['fields']['provider_name']['table'] = 'elobbyist_email_provider';
  $handler->display->display_options['fields']['provider_name']['field'] = 'name';
  $handler->display->display_options['fields']['provider_name']['relationship'] = 'providerid';
  $handler->display->display_options['fields']['provider_name']['label'] = t('Provider');
  /* Field: Elobbyist Contact: Bounce */
  $handler->display->display_options['fields']['bounce']['id'] = 'bounce';
  $handler->display->display_options['fields']['bounce']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['bounce']['field'] = 'bounce';
  $handler->display->display_options['fields']['bounce']['label'] = t('Status');
  /* Field: Elobbyist Contact: Subscriber Type */
  $handler->display->display_options['fields']['stype']['id'] = 'stype';
  $handler->display->display_options['fields']['stype']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['stype']['field'] = 'stype';
  $handler->display->display_options['fields']['stype']['label'] = t('Subscription');
  /* Field: Elobbyist Contact: Notify */
  $handler->display->display_options['fields']['notify']['id'] = 'notify';
  $handler->display->display_options['fields']['notify']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['notify']['field'] = 'notify';
  $handler->display->display_options['fields']['notify']['not'] = 0;
  /* Field: Elobbyist Contact: Edit */
  $handler->display->display_options['fields']['edit_link']['id'] = 'edit_link';
  $handler->display->display_options['fields']['edit_link']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['edit_link']['field'] = 'edit_link';
  /* Filter criterion: Elobbyist Contact: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'elobbyist_contact';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = 'subscriber';
  
  /* Filter criterion: Global: Combine fields filter */
  $handler->display->display_options['filters']['combine']['id'] = 'combine';
  $handler->display->display_options['filters']['combine']['table'] = 'views';
  $handler->display->display_options['filters']['combine']['field'] = 'combine';
  $handler->display->display_options['filters']['combine']['operator'] = 'contains';
  $handler->display->display_options['filters']['combine']['exposed'] = TRUE;
  $handler->display->display_options['filters']['combine']['expose']['operator_id'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['label'] = t('Search');
  $handler->display->display_options['filters']['combine']['expose']['operator'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['identifier'] = 'name_email';
  $handler->display->display_options['filters']['combine']['fields'] = array(
    'name' => 'name',
    'email' => 'email',
  );
  $handler->display->display_options['path'] =  'elobbyist/subscribers';
  
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['options'] = array();
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 0;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view email lobbyist contacts';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%view_%timestamp-yyyy_%timestamp-mm_%timestamp-dd.csv';
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['path'] = 'elobbyist/subscribers/download';

  $views[$view->name] = $view;
  
  //-----------------------
  //----------------------- Recipients
  //-----------------------
  
  $view = new view();
  $view->name = 'elobbyist_recipients';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'elobbyist_contact';
  $view->human_name = t('Email Lobbyist Recipients');
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view email lobbyist contacts';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = t('Go');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = $items_per_page;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'email' => 'email',
    'edit_link' => 'edit_link',
  );
  $handler->display->display_options['style_options']['default'] = 'name';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'email' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_link' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  
  /* Field: Bulk operations: Elobbyist Contact */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => t('Delete'),
    ),
    'rules_component::elobbyist_add_contacts_to_group' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 0,
      'label' => '',
    ),
  );
  
  /* Field: Elobbyist Contact: Contact id */
  $handler->display->display_options['fields']['cid']['id'] = 'cid';
  $handler->display->display_options['fields']['cid']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['cid']['field'] = 'cid';
  $handler->display->display_options['fields']['cid']['exclude'] = TRUE;
  /* Field: Elobbyist Contact: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = t('Full Name');
  /* Field: Elobbyist Contact: Email */
  $handler->display->display_options['fields']['email']['id'] = 'email';
  $handler->display->display_options['fields']['email']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['email']['field'] = 'email';
  $handler->display->display_options['fields']['email']['label'] = t('Email');
  /* Field: Elobbyist Contact: Edit */
  $handler->display->display_options['fields']['edit_link']['id'] = 'edit_link';
  $handler->display->display_options['fields']['edit_link']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['edit_link']['field'] = 'edit_link';
  /* Filter criterion: Elobbyist Contact: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'elobbyist_contact';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = 'recipient';
  
  /* Filter criterion: Global: Combine fields filter */
  $handler->display->display_options['filters']['combine']['id'] = 'combine';
  $handler->display->display_options['filters']['combine']['table'] = 'views';
  $handler->display->display_options['filters']['combine']['field'] = 'combine';
  $handler->display->display_options['filters']['combine']['operator'] = 'contains';
  $handler->display->display_options['filters']['combine']['exposed'] = TRUE;
  $handler->display->display_options['filters']['combine']['expose']['operator_id'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['label'] = t('Search');
  $handler->display->display_options['filters']['combine']['expose']['operator'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['identifier'] = 'name_email';
  $handler->display->display_options['filters']['combine']['fields'] = array(
    'name' => 'name',
    'email' => 'email',
  );
  $handler->display->display_options['path'] =  'elobbyist/recipients';
  
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['options'] = array();
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view email lobbyist contacts';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 0;
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%view_%timestamp-yyyy_%timestamp-mm_%timestamp-dd.csv';
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['path'] = 'elobbyist/recipients/download';

  $views[$view->name] = $view;
  
  //-----------------------
  //----------------------- Subscribers Groups
  //-----------------------
  
  $view = new view();
  $view->name = 'elobbyist_subscribers_groups';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'elobbyist_group';
  $view->human_name = t('Email Lobbyist Subscribers Groups');
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view email lobbyist contact groups';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = t('Go');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = $items_per_page;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'grpid' => 'grpid',
    'name' => 'name',
    'status' => 'status',
    'edit_link' => 'edit_link',
    'members_link' => 'members_link',
  );
  $handler->display->display_options['style_options']['default'] = 'name';
  $handler->display->display_options['style_options']['info'] = array(
    'grpid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_link' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'members_link' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  
  
  /* Field: Bulk operations */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'elobbyist_group';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => t('Delete'),
    ),
  );
  
  /* Field: Elobbyist Group: Group id */
  $handler->display->display_options['fields']['grpid']['id'] = 'grpid';
  $handler->display->display_options['fields']['grpid']['table'] = 'elobbyist_group';
  $handler->display->display_options['fields']['grpid']['field'] = 'grpid';
  $handler->display->display_options['fields']['grpid']['exclude'] = TRUE;
  /* Field: Elobbyist Group: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'elobbyist_group';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = t('Group Name');
  /* Field: Elobbyist Group: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'elobbyist_group';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  /* Field: Elobbyist Group: Edit */
  $handler->display->display_options['fields']['edit_link']['id'] = 'edit_link';
  $handler->display->display_options['fields']['edit_link']['table'] = 'elobbyist_group';
  $handler->display->display_options['fields']['edit_link']['field'] = 'edit_link';  
  /* Field: Elobbyist Group: Members */
  $handler->display->display_options['fields']['members_link']['id'] = 'members_link';
  $handler->display->display_options['fields']['members_link']['table'] = 'elobbyist_group';
  $handler->display->display_options['fields']['members_link']['field'] = 'members_link';

  /* Filter criterion: Elobbyist Group: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'elobbyist_group';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = 'subscribers';
  
  /* Filter criterion: Elobbyist Group: Name */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'elobbyist_group';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['operator'] = 'contains';
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  
  $handler->display->display_options['path'] = 'elobbyist/subscribers/groups';
  
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 0;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view email lobbyist contact groups';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%view_%timestamp-yyyy_%timestamp-mm_%timestamp-dd.csv';
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['path'] = 'elobbyist/subscribers/groups/download';

  $views[$view->name] = $view;
  
  //-----------------------
  //----------------------- Recipients Groups
  //-----------------------
  
  $view = new view();
  $view->name = 'elobbyist_recipients_groups';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'elobbyist_group';
  $view->human_name = t('Email Lobbyist Recipients Groups');
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view email lobbyist contact groups';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = t('Go');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = $items_per_page;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'grpid' => 'grpid',
    'name' => 'name',
    'status' => 'status',
    'edit_link' => 'edit_link',
    'members_link' => 'members_link',
  );
  $handler->display->display_options['style_options']['default'] = 'name';
  $handler->display->display_options['style_options']['info'] = array(
    'grpid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_link' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  
  /* Field: Bulk operations */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'elobbyist_group';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => t('Delete'),
    ),
  );
  
  /* Field: Elobbyist Group: Group id */
  $handler->display->display_options['fields']['grpid']['id'] = 'grpid';
  $handler->display->display_options['fields']['grpid']['table'] = 'elobbyist_group';
  $handler->display->display_options['fields']['grpid']['field'] = 'grpid';
  $handler->display->display_options['fields']['grpid']['exclude'] = TRUE;
  /* Field: Elobbyist Group: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'elobbyist_group';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = t('Group Name');
  /* Field: Elobbyist Group: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'elobbyist_group';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  /* Field: Elobbyist Group: Edit */
  $handler->display->display_options['fields']['edit_link']['id'] = 'edit_link';
  $handler->display->display_options['fields']['edit_link']['table'] = 'elobbyist_group';
  $handler->display->display_options['fields']['edit_link']['field'] = 'edit_link';
  /* Field: Elobbyist Group: Members */
  $handler->display->display_options['fields']['members_link']['id'] = 'members_link';
  $handler->display->display_options['fields']['members_link']['table'] = 'elobbyist_group';
  $handler->display->display_options['fields']['members_link']['field'] = 'members_link';

  /* Filter criterion: Elobbyist Group: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'elobbyist_group';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = 'recipients';
  
  /* Filter criterion: Elobbyist Group: Name */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'elobbyist_group';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['operator'] = 'contains';
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  
  $handler->display->display_options['path'] = 'elobbyist/recipients/groups';
  
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view email lobbyist contact groups';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 0;
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%view_%timestamp-yyyy_%timestamp-mm_%timestamp-dd.csv';
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['path'] = 'elobbyist/recipients/groups/download';

  $views[$view->name] = $view;
  
  //-----------------------
  //----------------------- Templates
  //-----------------------
  
  $view = new view();
  $view->name = 'elobbyist_templates';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'elobbyist_template';
  $view->human_name = t('Email Lobbyist Templates');
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view email lobbyist templates';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = t('Go');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = $items_per_page;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'tid' => 'tid',
    'name' => 'name',
    'subject' => 'subject',
    'status' => 'status',
    'edit_link' => 'edit_link',
  );
  $handler->display->display_options['style_options']['default'] = 'name';
  $handler->display->display_options['style_options']['info'] = array(
    'tid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'subject' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_link' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  
  /* Field: Bulk operations */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'elobbyist_template';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => t('Delete'),
    ),
  );
  
  /* Field: Elobbyist Template: Template id */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'elobbyist_template';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  /* Field: Elobbyist Template: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'elobbyist_template';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = t('Template Name');
  /* Field: Elobbyist Template: Subject */
  $handler->display->display_options['fields']['subject']['id'] = 'subject';
  $handler->display->display_options['fields']['subject']['table'] = 'elobbyist_template';
  $handler->display->display_options['fields']['subject']['field'] = 'subject';
  /* Field: Elobbyist Template: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'elobbyist_template';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  /* Field: Elobbyist Template: Edit */
  $handler->display->display_options['fields']['edit_link']['id'] = 'edit_link';
  $handler->display->display_options['fields']['edit_link']['table'] = 'elobbyist_template';
  $handler->display->display_options['fields']['edit_link']['field'] = 'edit_link';
  
  /* Filter criterion: Global: Combine fields filter */
  $handler->display->display_options['filters']['combine']['id'] = 'combine';
  $handler->display->display_options['filters']['combine']['table'] = 'views';
  $handler->display->display_options['filters']['combine']['field'] = 'combine';
  $handler->display->display_options['filters']['combine']['operator'] = 'contains';
  $handler->display->display_options['filters']['combine']['exposed'] = TRUE;
  $handler->display->display_options['filters']['combine']['expose']['operator_id'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['label'] = t('Search');
  $handler->display->display_options['filters']['combine']['expose']['operator'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['identifier'] = 'name_subject';
  $handler->display->display_options['filters']['combine']['fields'] = array(
    'name' => 'name',
    'subject' => 'subject',
  );
  $handler->display->display_options['path'] =  'elobbyist/templates';
  
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 0;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view email lobbyist templates';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%view_%timestamp-yyyy_%timestamp-mm_%timestamp-dd.csv';
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['path'] = 'elobbyist/templates/download';

  $views[$view->name] = $view;
  
  //-----------------------
  //----------------------- Mailings
  //-----------------------  
  
  $view = new view();
  $view->name = 'elobbyist_mailings';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'elobbyist_mailing';
  $view->human_name = t('Email Lobbyist Mailings');
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view email lobbyist mailings';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = t('Go');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = $items_per_page;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'mid' => 'mid',
    'name' => 'name',
    'template_name' => 'template_name',
    'status' => 'status',
    'schedule' => 'schedule',
    'start' => 'start',
    'end' => 'end',
    'edit_link' => 'edit_link',
    'tid' => 'tid',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'mid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'template_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'schedule' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'start' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'end' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_link' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'tid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  
  /* Field: Bulk operations */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'elobbyist_mailing';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => t('Delete'),
    ),
  );
  
  /* Relationship: Elobbyist Mailing: Template */
  $handler->display->display_options['relationships']['tid']['id'] = 'tid';
  $handler->display->display_options['relationships']['tid']['table'] = 'elobbyist_mailing';
  $handler->display->display_options['relationships']['tid']['field'] = 'tid';
  /* Field: Elobbyist Mailing: Mailing id */
  $handler->display->display_options['fields']['mid']['id'] = 'mid';
  $handler->display->display_options['fields']['mid']['table'] = 'elobbyist_mailing';
  $handler->display->display_options['fields']['mid']['field'] = 'mid';
  $handler->display->display_options['fields']['mid']['exclude'] = TRUE;
  /* Field: Elobbyist Mailing: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'elobbyist_mailing';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = t('Mailing Name');
  /* Field: Elobbyist Template: Name */
  $handler->display->display_options['fields']['template_name']['id'] = 'template_name';
  $handler->display->display_options['fields']['template_name']['table'] = 'elobbyist_template';
  $handler->display->display_options['fields']['template_name']['field'] = 'name';
  $handler->display->display_options['fields']['template_name']['relationship'] = 'tid';
  $handler->display->display_options['fields']['template_name']['label'] = t('Template');
  /* Field: Elobbyist Mailing: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'elobbyist_mailing';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  /* Field: Elobbyist Mailing: Schedule */
  $handler->display->display_options['fields']['schedule']['id'] = 'schedule';
  $handler->display->display_options['fields']['schedule']['table'] = 'elobbyist_mailing';
  $handler->display->display_options['fields']['schedule']['field'] = 'schedule';
  /* Field: Elobbyist Mailing: Start */
  $handler->display->display_options['fields']['start']['id'] = 'start';
  $handler->display->display_options['fields']['start']['table'] = 'elobbyist_mailing';
  $handler->display->display_options['fields']['start']['field'] = 'start';
  $handler->display->display_options['fields']['start']['date_format'] = 'custom';
  $handler->display->display_options['fields']['start']['custom_date_format'] = 'Y-m-d G:i';
  /* Field: Elobbyist Mailing: End */
  $handler->display->display_options['fields']['end']['id'] = 'end';
  $handler->display->display_options['fields']['end']['table'] = 'elobbyist_mailing';
  $handler->display->display_options['fields']['end']['field'] = 'end';
  $handler->display->display_options['fields']['end']['date_format'] = 'custom';
  $handler->display->display_options['fields']['end']['custom_date_format'] = 'Y-m-d G:i';
  /* Field: Elobbyist Mailing: Edit */
  $handler->display->display_options['fields']['edit_link']['id'] = 'edit_link';
  $handler->display->display_options['fields']['edit_link']['table'] = 'elobbyist_mailing';
  $handler->display->display_options['fields']['edit_link']['field'] = 'edit_link';
  /* Field: Elobbyist Template: Template id */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'elobbyist_template';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['relationship'] = 'tid';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  /* Sort criterion: Elobbyist Mailing: Created */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'elobbyist_mailing';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  
  /* Filter criterion: Global: Combine fields filter */
  $handler->display->display_options['filters']['combine']['id'] = 'combine';
  $handler->display->display_options['filters']['combine']['table'] = 'views';
  $handler->display->display_options['filters']['combine']['field'] = 'combine';
  $handler->display->display_options['filters']['combine']['operator'] = 'contains';
  $handler->display->display_options['filters']['combine']['exposed'] = TRUE;
  $handler->display->display_options['filters']['combine']['expose']['operator_id'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['label'] = t('Search');
  $handler->display->display_options['filters']['combine']['expose']['operator'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['identifier'] = 'name_template';
  $handler->display->display_options['filters']['combine']['fields'] = array(
    'name' => 'name',
    'template_name' => 'template_name',
  );
  $handler->display->display_options['path'] =  'elobbyist/mailings';
  
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 0;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view email lobbyist mailings';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%view_%timestamp-yyyy_%timestamp-mm_%timestamp-dd.csv';
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['path'] = 'elobbyist/mailings/download';

  $views[$view->name] = $view;
  

  //-----------------------
  //----------------------- Email Providers
  //-----------------------  
  
  $view = new view();
  $view->name = 'elobbyist_providers';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'elobbyist_email_provider';
  $view->human_name = t('Email Lobbyist Providers');
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view email lobbyist email providers';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = t('Go');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = $items_per_page;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'providerid' => 'providerid',
    'edit_link' => 'edit_link',
    'name' => 'name',
    'smtphost' => 'smtphost',
    'smtpport' => 'smtpport',
  );
  $handler->display->display_options['style_options']['default'] = 'name';
  $handler->display->display_options['style_options']['info'] = array(
    'providerid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_link' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'smtphost' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'smtpport' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  
  
  /* Field: Bulk operations */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'elobbyist_email_provider';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => t('Delete'),
    ),
  );
  
  /* Field: Elobbyist Email Provider: Provider id */
  $handler->display->display_options['fields']['providerid']['id'] = 'providerid';
  $handler->display->display_options['fields']['providerid']['table'] = 'elobbyist_email_provider';
  $handler->display->display_options['fields']['providerid']['field'] = 'providerid';
  $handler->display->display_options['fields']['providerid']['exclude'] = TRUE;
  /* Field: Elobbyist Email Provider: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'elobbyist_email_provider';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = t('Provider Name');
  /* Field: Elobbyist Email Provider: Smtp host */
  $handler->display->display_options['fields']['smtphost']['id'] = 'smtphost';
  $handler->display->display_options['fields']['smtphost']['table'] = 'elobbyist_email_provider';
  $handler->display->display_options['fields']['smtphost']['field'] = 'smtphost';
  $handler->display->display_options['fields']['smtphost']['label'] = t('SMTP Server');
  /* Field: Elobbyist Email Provider: Smtp port */
  $handler->display->display_options['fields']['smtpport']['id'] = 'smtpport';
  $handler->display->display_options['fields']['smtpport']['table'] = 'elobbyist_email_provider';
  $handler->display->display_options['fields']['smtpport']['field'] = 'smtpport';
  $handler->display->display_options['fields']['smtpport']['label'] = t('Port');
  /* Field: Email Lobbyist Email Provider: Domain Name */
  $handler->display->display_options['fields']['elobbyist_domain']['id'] = 'elobbyist_domain';
  $handler->display->display_options['fields']['elobbyist_domain']['table'] = 'field_data_elobbyist_domain';
  $handler->display->display_options['fields']['elobbyist_domain']['field'] = 'elobbyist_domain';
  $handler->display->display_options['fields']['elobbyist_domain']['label'] = 'Domain Names';
  /* Field: Elobbyist Email Provider: Edit */
  $handler->display->display_options['fields']['edit_link']['id'] = 'edit_link';
  $handler->display->display_options['fields']['edit_link']['table'] = 'elobbyist_email_provider';
  $handler->display->display_options['fields']['edit_link']['field'] = 'edit_link';
  
  /* Filter criterion: Global: Combine fields filter */
  $handler->display->display_options['filters']['combine']['id'] = 'combine';
  $handler->display->display_options['filters']['combine']['table'] = 'views';
  $handler->display->display_options['filters']['combine']['field'] = 'combine';
  $handler->display->display_options['filters']['combine']['operator'] = 'contains';
  $handler->display->display_options['filters']['combine']['exposed'] = TRUE;
  $handler->display->display_options['filters']['combine']['expose']['operator_id'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['label'] = t('Search');
  $handler->display->display_options['filters']['combine']['expose']['operator'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['identifier'] = 'name_host_domain';
  $handler->display->display_options['filters']['combine']['fields'] = array(
    'name' => 'name',
    'smtphost' => 'smtphost',
    'elobbyist_domain' => 'elobbyist_domain',
  );
  $handler->display->display_options['path'] =  'elobbyist/email-providers';

  
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 0;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view email lobbyist email providers';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%view_%timestamp-yyyy_%timestamp-mm_%timestamp-dd.csv';
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['path'] = 'elobbyist/email-providers/download';

  $views[$view->name] = $view;
  
  //-----------------------
  //----------------------- Subscribers By Group
  //-----------------------  
  
  $view = new view();
  $view->name = 'elobbyist_subscribers_by_group';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'elobbyist_contact';
  $view->human_name = t('Email Lobbyist Subscribers By Group');
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view email lobbyist contacts';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = t('Go');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = $items_per_page;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'email' => 'email',
    'bounce' => 'bounce',
    'stype' => 'stype',
    'edit_link' => 'edit_link',
    'provider_name' => 'provider_name',
    'notify' => 'notify',
  );
  $handler->display->display_options['style_options']['default'] = 'name';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'email' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'bounce' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'stype' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_link' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'provider_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  
  /* Field: Bulk operations */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => t('Delete'),
    ),
    'rules_component::elobbyist_remove_contacts_from_group' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
   
  /* Relationship: Elobbyist Contact: Relation: Group Subscribers (elobbyist_contact → relation) */
  $handler->display->display_options['relationships']['relation_base_left_elobbyist_subscriber']['id'] = 'relation_base_left_elobbyist_subscriber';
  $handler->display->display_options['relationships']['relation_base_left_elobbyist_subscriber']['table'] = 'elobbyist_contact';
  $handler->display->display_options['relationships']['relation_base_left_elobbyist_subscriber']['field'] = 'relation_base_left_elobbyist_subscriber';
  $handler->display->display_options['relationships']['relation_base_left_elobbyist_subscriber']['label'] = 'Contact to Relation';
  $handler->display->display_options['relationships']['relation_base_left_elobbyist_subscriber']['required'] = TRUE;
  $handler->display->display_options['relationships']['relation_base_left_elobbyist_subscriber']['entity_deduplication_left'] = 0;
  /* Relationship: Relation: Relation: Group Subscribers (relation → elobbyist_group) */
  $handler->display->display_options['relationships']['relation_base_elobbyist_subscriber_elobbyist_group']['id'] = 'relation_base_elobbyist_subscriber_elobbyist_group';
  $handler->display->display_options['relationships']['relation_base_elobbyist_subscriber_elobbyist_group']['table'] = 'relation';
  $handler->display->display_options['relationships']['relation_base_elobbyist_subscriber_elobbyist_group']['field'] = 'relation_base_elobbyist_subscriber_elobbyist_group';
  $handler->display->display_options['relationships']['relation_base_elobbyist_subscriber_elobbyist_group']['relationship'] = 'relation_base_left_elobbyist_subscriber';
  $handler->display->display_options['relationships']['relation_base_elobbyist_subscriber_elobbyist_group']['label'] = 'Relation to Group';
  $handler->display->display_options['relationships']['relation_base_elobbyist_subscriber_elobbyist_group']['required'] = TRUE;
  $handler->display->display_options['relationships']['relation_base_elobbyist_subscriber_elobbyist_group']['entity_deduplication_right'] = 0;
  
  /* Field: Elobbyist Contact: Contact id */
  $handler->display->display_options['fields']['cid']['id'] = 'cid';
  $handler->display->display_options['fields']['cid']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['cid']['field'] = 'cid';
  $handler->display->display_options['fields']['cid']['exclude'] = TRUE;
  /* Field: Elobbyist Contact: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = t('Full Name');
  /* Field: Elobbyist Contact: Email */
  $handler->display->display_options['fields']['email']['id'] = 'email';
  $handler->display->display_options['fields']['email']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['email']['field'] = 'email';
  /* Field: Elobbyist Contact: Bounce */
  $handler->display->display_options['fields']['bounce']['id'] = 'bounce';
  $handler->display->display_options['fields']['bounce']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['bounce']['field'] = 'bounce';
  $handler->display->display_options['fields']['bounce']['label'] = t('Status');
  /* Field: Elobbyist Contact: Subscriber Type */
  $handler->display->display_options['fields']['stype']['id'] = 'stype';
  $handler->display->display_options['fields']['stype']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['stype']['field'] = 'stype';
  $handler->display->display_options['fields']['stype']['label'] = t('Subscription');
  /* Field: Elobbyist Contact: Notify */
  $handler->display->display_options['fields']['notify']['id'] = 'notify';
  $handler->display->display_options['fields']['notify']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['notify']['field'] = 'notify';
  $handler->display->display_options['fields']['notify']['not'] = 0;
  /* Field: Elobbyist Contact: Edit */
  $handler->display->display_options['fields']['edit_link']['id'] = 'edit_link';
  $handler->display->display_options['fields']['edit_link']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['edit_link']['field'] = 'edit_link';
  
  /* Field: Relation: Rid */
  $handler->display->display_options['fields']['rid']['id'] = 'rid';
  $handler->display->display_options['fields']['rid']['table'] = 'relation';
  $handler->display->display_options['fields']['rid']['field'] = 'rid';
  $handler->display->display_options['fields']['rid']['relationship'] = 'relation_base_left_elobbyist_subscriber';
  $handler->display->display_options['fields']['rid']['exclude'] = TRUE;
  
  /* Contextual filter: Elobbyist Group: Group id */
  $handler->display->display_options['arguments']['grpid']['id'] = 'grpid';
  $handler->display->display_options['arguments']['grpid']['table'] = 'elobbyist_group';
  $handler->display->display_options['arguments']['grpid']['field'] = 'grpid';
  $handler->display->display_options['arguments']['grpid']['relationship'] = 'relation_base_elobbyist_subscriber_elobbyist_group';
  $handler->display->display_options['arguments']['grpid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['grpid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['grpid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['grpid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['grpid']['summary_options']['items_per_page'] = $items_per_page;
    
  /* Filter criterion: Elobbyist Contact: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'elobbyist_contact';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = 'subscriber';
  $handler->display->display_options['filters']['type']['group'] = 1;
  
  /* Filter criterion: Global: Combine fields filter */
  $handler->display->display_options['filters']['combine']['id'] = 'combine';
  $handler->display->display_options['filters']['combine']['table'] = 'views';
  $handler->display->display_options['filters']['combine']['field'] = 'combine';
  $handler->display->display_options['filters']['combine']['operator'] = 'contains';
  $handler->display->display_options['filters']['combine']['exposed'] = TRUE;
  $handler->display->display_options['filters']['combine']['expose']['operator_id'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['label'] = t('Search');
  $handler->display->display_options['filters']['combine']['expose']['operator'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['identifier'] = 'name_email';
  $handler->display->display_options['filters']['combine']['fields'] = array(
    'name' => 'name',
    'email' => 'email',
  );
  $handler->display->display_options['path'] = 'elobbyist/contact-group/%/members';
  
  $views[$view->name] = $view;
  
  //-----------------------
  //----------------------- Recipients By Group
  //-----------------------  
  
  $view = new view();
  $view->name = 'elobbyist_recipients_by_group';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'elobbyist_contact';
  $view->human_name = t('Email Lobbyist Recipients By Group');
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view email lobbyist contacts';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = t('Go');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = $items_per_page;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'email' => 'email',
    'edit_link' => 'edit_link',
  );
  $handler->display->display_options['style_options']['default'] = 'name';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'email' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_link' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  
  /* Field: Bulk operations */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => t('Delete'),
    ),
    'rules_component::elobbyist_remove_contacts_from_group' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
   
  /* Relationship: Elobbyist Contact: Relation: Group Recipients (elobbyist_contact → relation) */
  $handler->display->display_options['relationships']['relation_base_left_elobbyist_recipient']['id'] = 'relation_base_left_elobbyist_recipient';
  $handler->display->display_options['relationships']['relation_base_left_elobbyist_recipient']['table'] = 'elobbyist_contact';
  $handler->display->display_options['relationships']['relation_base_left_elobbyist_recipient']['field'] = 'relation_base_left_elobbyist_recipient';
  $handler->display->display_options['relationships']['relation_base_left_elobbyist_recipient']['label'] = 'Contact to Relation';
  $handler->display->display_options['relationships']['relation_base_left_elobbyist_recipient']['required'] = TRUE;
  $handler->display->display_options['relationships']['relation_base_left_elobbyist_recipient']['entity_deduplication_left'] = 0;
  /* Relationship: Relation: Relation: Group Recipients (relation → elobbyist_group) */
  $handler->display->display_options['relationships']['relation_base_elobbyist_recipient_elobbyist_group']['id'] = 'relation_base_elobbyist_recipient_elobbyist_group';
  $handler->display->display_options['relationships']['relation_base_elobbyist_recipient_elobbyist_group']['table'] = 'relation';
  $handler->display->display_options['relationships']['relation_base_elobbyist_recipient_elobbyist_group']['field'] = 'relation_base_elobbyist_recipient_elobbyist_group';
  $handler->display->display_options['relationships']['relation_base_elobbyist_recipient_elobbyist_group']['relationship'] = 'relation_base_left_elobbyist_recipient';
  $handler->display->display_options['relationships']['relation_base_elobbyist_recipient_elobbyist_group']['label'] = 'Relation to Group';
  $handler->display->display_options['relationships']['relation_base_elobbyist_recipient_elobbyist_group']['required'] = TRUE;
  $handler->display->display_options['relationships']['relation_base_elobbyist_recipient_elobbyist_group']['entity_deduplication_right'] = 0;
  
  /* Field: Elobbyist Contact: Contact id */
  $handler->display->display_options['fields']['cid']['id'] = 'cid';
  $handler->display->display_options['fields']['cid']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['cid']['field'] = 'cid';
  $handler->display->display_options['fields']['cid']['exclude'] = TRUE;
  /* Field: Elobbyist Contact: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = t('Full Name');
  /* Field: Elobbyist Contact: Email */
  $handler->display->display_options['fields']['email']['id'] = 'email';
  $handler->display->display_options['fields']['email']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['email']['field'] = 'email';
  /* Field: Elobbyist Contact: Edit */
  $handler->display->display_options['fields']['edit_link']['id'] = 'edit_link';
  $handler->display->display_options['fields']['edit_link']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['edit_link']['field'] = 'edit_link';
  /* Field: Relation: Rid */
  $handler->display->display_options['fields']['rid']['id'] = 'rid';
  $handler->display->display_options['fields']['rid']['table'] = 'relation';
  $handler->display->display_options['fields']['rid']['field'] = 'rid';
  $handler->display->display_options['fields']['rid']['relationship'] = 'relation_base_left_elobbyist_recipient';
  $handler->display->display_options['fields']['rid']['exclude'] = TRUE;
  
  /* Contextual filter: Elobbyist Group: Group id */
  $handler->display->display_options['arguments']['grpid']['id'] = 'grpid';
  $handler->display->display_options['arguments']['grpid']['table'] = 'elobbyist_group';
  $handler->display->display_options['arguments']['grpid']['field'] = 'grpid';
  $handler->display->display_options['arguments']['grpid']['relationship'] = 'relation_base_elobbyist_recipient_elobbyist_group';
  $handler->display->display_options['arguments']['grpid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['grpid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['grpid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['grpid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['grpid']['summary_options']['items_per_page'] = $items_per_page;
  
  /* Filter criterion: Elobbyist Contact: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'elobbyist_contact';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = 'recipient';
  $handler->display->display_options['filters']['type']['group'] = 1;
  
  /* Filter criterion: Global: Combine fields filter */
  $handler->display->display_options['filters']['combine']['id'] = 'combine';
  $handler->display->display_options['filters']['combine']['table'] = 'views';
  $handler->display->display_options['filters']['combine']['field'] = 'combine';
  $handler->display->display_options['filters']['combine']['operator'] = 'contains';
  $handler->display->display_options['filters']['combine']['exposed'] = TRUE;
  $handler->display->display_options['filters']['combine']['expose']['operator_id'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['label'] = t('Search');
  $handler->display->display_options['filters']['combine']['expose']['operator'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['identifier'] = 'name_email';
  $handler->display->display_options['filters']['combine']['fields'] = array(
    'name' => 'name',
    'email' => 'email',
  );
  $handler->display->display_options['path'] =  'elobbyist/contact-group/%/members';
 
  $views[$view->name] = $view;
  
  //-----------------------
  //----------------------- Activities By Subscriber
  //-----------------------  
  
  $view = new view();
  $view->name = 'elobbyist_activities_by_subscriber';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'elobbyist_activity';
  $view->human_name = t('Email Lobbyist Activities By Subscriber');
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view email lobbyist activities';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = t('Go');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = $items_per_page;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'aid' => 'aid',
    'name' => 'name',
    'email' => 'email',
    'subject' => 'subject',
    'mailing_name' => 'mailing_name',
    'mid' => 'mid',
    'tid' => 'tid',
    'view_link' => 'view_link',
    'created' => 'created',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'aid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'email' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'subject' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mailing_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'view_link' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'tid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Elobbyist Activity: Mailing */
  $handler->display->display_options['relationships']['mid']['id'] = 'mid';
  $handler->display->display_options['relationships']['mid']['table'] = 'elobbyist_activity';
  $handler->display->display_options['relationships']['mid']['field'] = 'mid';
  $handler->display->display_options['relationships']['mid']['required'] = TRUE;
  /* Relationship: Elobbyist Activity: Template */
  $handler->display->display_options['relationships']['tid']['id'] = 'tid';
  $handler->display->display_options['relationships']['tid']['table'] = 'elobbyist_activity';
  $handler->display->display_options['relationships']['tid']['field'] = 'tid';
  /* Relationship: Elobbyist Activity: To */
  $handler->display->display_options['relationships']['tocid']['id'] = 'tocid';
  $handler->display->display_options['relationships']['tocid']['table'] = 'elobbyist_activity';
  $handler->display->display_options['relationships']['tocid']['field'] = 'tocid';
  $handler->display->display_options['relationships']['tocid']['label'] = 'tocid';
  $handler->display->display_options['relationships']['tocid']['required'] = TRUE;
  /* Field: Elobbyist Activity: Activity id */
  $handler->display->display_options['fields']['aid']['id'] = 'aid';
  $handler->display->display_options['fields']['aid']['table'] = 'elobbyist_activity';
  $handler->display->display_options['fields']['aid']['field'] = 'aid';
  $handler->display->display_options['fields']['aid']['exclude'] = TRUE;
  /* Field: Elobbyist Contact: Contact id */
  $handler->display->display_options['fields']['cid']['id'] = 'cid';
  $handler->display->display_options['fields']['cid']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['cid']['field'] = 'cid';
  $handler->display->display_options['fields']['cid']['relationship'] = 'tocid';
  $handler->display->display_options['fields']['cid']['exclude'] = TRUE;
  
  /* Field: Elobbyist Contact: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'tocid';
  $handler->display->display_options['fields']['name']['label'] = t('Recipient');
  /* Field: Elobbyist Contact: Email */
  $handler->display->display_options['fields']['email']['id'] = 'email';
  $handler->display->display_options['fields']['email']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['email']['field'] = 'email';
  $handler->display->display_options['fields']['email']['relationship'] = 'tocid';
  /* Field: Elobbyist Template: Subject */
  $handler->display->display_options['fields']['subject']['id'] = 'subject';
  $handler->display->display_options['fields']['subject']['table'] = 'elobbyist_template';
  $handler->display->display_options['fields']['subject']['field'] = 'subject';
  $handler->display->display_options['fields']['subject']['relationship'] = 'tid';
  /* Field: Elobbyist Mailing: Name */
  $handler->display->display_options['fields']['mailing_name']['id'] = 'mailing_name';
  $handler->display->display_options['fields']['mailing_name']['table'] = 'elobbyist_mailing';
  $handler->display->display_options['fields']['mailing_name']['field'] = 'name';
  $handler->display->display_options['fields']['mailing_name']['relationship'] = 'mid';
  $handler->display->display_options['fields']['mailing_name']['label'] = t('Mailing');
  
  /* Field: Elobbyist Activity: View */
  $handler->display->display_options['fields']['view_link']['id'] = 'view_link';
  $handler->display->display_options['fields']['view_link']['table'] = 'elobbyist_activity';
  $handler->display->display_options['fields']['view_link']['field'] = 'view_link';
  $handler->display->display_options['fields']['view_link']['label'] = t('View Email');
  /* Field: Elobbyist Activity: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'elobbyist_activity';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = t('Sent');
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'Y-m-d G:i';
  /* Field: Elobbyist Activity: Mailing id */
  $handler->display->display_options['fields']['mid']['id'] = 'mid';
  $handler->display->display_options['fields']['mid']['table'] = 'elobbyist_activity';
  $handler->display->display_options['fields']['mid']['field'] = 'mid';
  $handler->display->display_options['fields']['mid']['exclude'] = TRUE;
  /* Field: Elobbyist Activity: Template id */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'elobbyist_activity';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  
  /* Contextual filter: Elobbyist Group: Group id */
  $handler->display->display_options['arguments']['fromcid']['id'] = 'fromcid';
  $handler->display->display_options['arguments']['fromcid']['table'] = 'elobbyist_activity';
  $handler->display->display_options['arguments']['fromcid']['field'] = 'fromcid';
  $handler->display->display_options['arguments']['fromcid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['fromcid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['fromcid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['fromcid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['fromcid']['summary_options']['items_per_page'] = $items_per_page;
    
  /* Filter criterion: Global: Combine fields filter */
  $handler->display->display_options['filters']['combine']['id'] = 'combine';
  $handler->display->display_options['filters']['combine']['table'] = 'views';
  $handler->display->display_options['filters']['combine']['field'] = 'combine';
  $handler->display->display_options['filters']['combine']['operator'] = 'contains';
  $handler->display->display_options['filters']['combine']['exposed'] = TRUE;
  $handler->display->display_options['filters']['combine']['expose']['operator_id'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['label'] = t('Search');
  $handler->display->display_options['filters']['combine']['expose']['operator'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['identifier'] = 'act_fields';
  $handler->display->display_options['filters']['combine']['fields'] = array(
    'name' => 'name',
    'email' => 'email',
    'subject' => 'subject',
    'mailing_name' => 'mailing_name',
  );
  $handler->display->display_options['path'] =  'elobbyist/contact/%/emails';
  
  $views[$view->name] = $view;
  
  //-----------------------
  //----------------------- Activities By Recipient
  //-----------------------  
  
  $view = new view();
  $view->name = 'elobbyist_activities_by_recipient';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'elobbyist_activity';
  $view->human_name = t('Email Lobbyist Activities By Recipient');
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view email lobbyist activities';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = t('Go');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = $items_per_page;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'aid' => 'aid',
    'name' => 'name',
    'email' => 'email',
    'subject' => 'subject',
    'mailing_name' => 'mailing_name',
    'view_link' => 'view_link',
    'created' => 'created',
    'mid' => 'mid',
    'tid' => 'tid',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'aid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'email' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'subject' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mailing_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'view_link' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'tid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Elobbyist Activity: Mailing */
  $handler->display->display_options['relationships']['mid']['id'] = 'mid';
  $handler->display->display_options['relationships']['mid']['table'] = 'elobbyist_activity';
  $handler->display->display_options['relationships']['mid']['field'] = 'mid';
  $handler->display->display_options['relationships']['mid']['required'] = TRUE;
  /* Relationship: Elobbyist Activity: Template */
  $handler->display->display_options['relationships']['tid']['id'] = 'tid';
  $handler->display->display_options['relationships']['tid']['table'] = 'elobbyist_activity';
  $handler->display->display_options['relationships']['tid']['field'] = 'tid';
  /* Relationship: Elobbyist Activity: From */
  $handler->display->display_options['relationships']['fromcid']['id'] = 'fromcid';
  $handler->display->display_options['relationships']['fromcid']['table'] = 'elobbyist_activity';
  $handler->display->display_options['relationships']['fromcid']['field'] = 'fromcid';
  $handler->display->display_options['relationships']['fromcid']['label'] = 'fromcid';
  $handler->display->display_options['relationships']['fromcid']['required'] = TRUE;
  /* Field: Elobbyist Activity: Activity id */
  $handler->display->display_options['fields']['aid']['id'] = 'aid';
  $handler->display->display_options['fields']['aid']['table'] = 'elobbyist_activity';
  $handler->display->display_options['fields']['aid']['field'] = 'aid';
  $handler->display->display_options['fields']['aid']['exclude'] = TRUE;
  /* Field: Elobbyist Contact: Contact id */
  $handler->display->display_options['fields']['cid']['id'] = 'cid';
  $handler->display->display_options['fields']['cid']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['cid']['field'] = 'cid';
  $handler->display->display_options['fields']['cid']['relationship'] = 'fromcid';
  $handler->display->display_options['fields']['cid']['exclude'] = TRUE;
  
  /* Field: Elobbyist Contact: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'fromcid';
  $handler->display->display_options['fields']['name']['label'] = t('Sender');
  /* Field: Elobbyist Contact: Email */
  $handler->display->display_options['fields']['email']['id'] = 'email';
  $handler->display->display_options['fields']['email']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['email']['field'] = 'email';
  $handler->display->display_options['fields']['email']['relationship'] = 'fromcid';
  /* Field: Elobbyist Template: Subject */
  $handler->display->display_options['fields']['subject']['id'] = 'subject';
  $handler->display->display_options['fields']['subject']['table'] = 'elobbyist_template';
  $handler->display->display_options['fields']['subject']['field'] = 'subject';
  $handler->display->display_options['fields']['subject']['relationship'] = 'tid';
  /* Field: Elobbyist Mailing: Name */
  $handler->display->display_options['fields']['mailing_name']['id'] = 'mailing_name';
  $handler->display->display_options['fields']['mailing_name']['table'] = 'elobbyist_mailing';
  $handler->display->display_options['fields']['mailing_name']['field'] = 'name';
  $handler->display->display_options['fields']['mailing_name']['relationship'] = 'mid';
  $handler->display->display_options['fields']['mailing_name']['label'] = t('Mailing');
  
  /* Field: Elobbyist Activity: View */
  $handler->display->display_options['fields']['view_link']['id'] = 'view_link';
  $handler->display->display_options['fields']['view_link']['table'] = 'elobbyist_activity';
  $handler->display->display_options['fields']['view_link']['field'] = 'view_link';
  $handler->display->display_options['fields']['view_link']['label'] = t('View Email');
  /* Field: Elobbyist Activity: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'elobbyist_activity';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = t('Sent');
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'Y-m-d G:i';
  /* Field: Elobbyist Activity: Mailing id */
  $handler->display->display_options['fields']['mid']['id'] = 'mid';
  $handler->display->display_options['fields']['mid']['table'] = 'elobbyist_activity';
  $handler->display->display_options['fields']['mid']['field'] = 'mid';
  $handler->display->display_options['fields']['mid']['exclude'] = TRUE;
  /* Field: Elobbyist Activity: Template id */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'elobbyist_activity';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  
  /* Contextual filter: Elobbyist Group: Group id */
  $handler->display->display_options['arguments']['tocid']['id'] = 'tocid';
  $handler->display->display_options['arguments']['tocid']['table'] = 'elobbyist_activity';
  $handler->display->display_options['arguments']['tocid']['field'] = 'tocid';
  $handler->display->display_options['arguments']['tocid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['tocid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['tocid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['tocid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['tocid']['summary_options']['items_per_page'] = $items_per_page;

  
  /* Filter criterion: Global: Combine fields filter */
  $handler->display->display_options['filters']['combine']['id'] = 'combine';
  $handler->display->display_options['filters']['combine']['table'] = 'views';
  $handler->display->display_options['filters']['combine']['field'] = 'combine';
  $handler->display->display_options['filters']['combine']['operator'] = 'contains';
  $handler->display->display_options['filters']['combine']['exposed'] = TRUE;
  $handler->display->display_options['filters']['combine']['expose']['operator_id'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['label'] = t('Search');
  $handler->display->display_options['filters']['combine']['expose']['operator'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['identifier'] = 'act_fields';
  $handler->display->display_options['filters']['combine']['fields'] = array(
    'name' => 'name',
    'email' => 'email',
    'subject' => 'subject',
    'mailing_name' => 'mailing_name',
  );
  $handler->display->display_options['path'] =  'elobbyist/contact/%/emails';

  $views[$view->name] = $view;
  
  //-----------------------
  //----------------------- Activities By Mailing
  //-----------------------  
  
  $view = new view();
  $view->name = 'elobbyist_activities_by_mailing';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'elobbyist_activity';
  $view->human_name = t('Email Lobbyist Activities By Mailing');
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view email lobbyist activities';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = t('Go');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = $items_per_page;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'aid' => 'aid',
    'name' => 'name',
    'email' => 'email',
    'recipient_name' => 'recipient_name',
    'recipient_email' => 'recipient_email',
    'subject' => 'subject',
    'view_link' => 'view_link',
    'created' => 'created',
    'mid' => 'mid',
    'tid' => 'tid',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'aid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'email' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'recipient_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'recipient_email' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'subject' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'view_link' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'tid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Elobbyist Activity: Mailing */
  $handler->display->display_options['relationships']['mid']['id'] = 'mid';
  $handler->display->display_options['relationships']['mid']['table'] = 'elobbyist_activity';
  $handler->display->display_options['relationships']['mid']['field'] = 'mid';
  $handler->display->display_options['relationships']['mid']['required'] = TRUE;
  /* Relationship: Elobbyist Activity: Template */
  $handler->display->display_options['relationships']['tid']['id'] = 'tid';
  $handler->display->display_options['relationships']['tid']['table'] = 'elobbyist_activity';
  $handler->display->display_options['relationships']['tid']['field'] = 'tid';
  /* Relationship: Elobbyist Activity: From */
  $handler->display->display_options['relationships']['fromcid']['id'] = 'fromcid';
  $handler->display->display_options['relationships']['fromcid']['table'] = 'elobbyist_activity';
  $handler->display->display_options['relationships']['fromcid']['field'] = 'fromcid';
  $handler->display->display_options['relationships']['fromcid']['label'] = 'fromcid';
  $handler->display->display_options['relationships']['fromcid']['required'] = TRUE;
  /* Relationship: Elobbyist Activity: To */
  $handler->display->display_options['relationships']['tocid']['id'] = 'tocid';
  $handler->display->display_options['relationships']['tocid']['table'] = 'elobbyist_activity';
  $handler->display->display_options['relationships']['tocid']['field'] = 'tocid';
  $handler->display->display_options['relationships']['tocid']['label'] = 'tocid';
  $handler->display->display_options['relationships']['tocid']['required'] = TRUE;
  /* Field: Elobbyist Activity: Activity id */
  $handler->display->display_options['fields']['aid']['id'] = 'aid';
  $handler->display->display_options['fields']['aid']['table'] = 'elobbyist_activity';
  $handler->display->display_options['fields']['aid']['field'] = 'aid';
  $handler->display->display_options['fields']['aid']['exclude'] = TRUE;
  /* Field: Elobbyist Contact: Contact id */
  $handler->display->display_options['fields']['cid']['id'] = 'cid';
  $handler->display->display_options['fields']['cid']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['cid']['field'] = 'cid';
  $handler->display->display_options['fields']['cid']['relationship'] = 'fromcid';
  $handler->display->display_options['fields']['cid']['exclude'] = TRUE;
  /* Field: Elobbyist Contact: Contact id */
  $handler->display->display_options['fields']['cid_1']['id'] = 'cid_1';
  $handler->display->display_options['fields']['cid_1']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['cid_1']['field'] = 'cid';
  $handler->display->display_options['fields']['cid_1']['relationship'] = 'tocid';
  $handler->display->display_options['fields']['cid_1']['exclude'] = TRUE;
  
  /* Field: Elobbyist Contact: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'fromcid';
  $handler->display->display_options['fields']['name']['label'] = t('Sender');
  /* Field: Elobbyist Contact: Email */
  $handler->display->display_options['fields']['email']['id'] = 'email';
  $handler->display->display_options['fields']['email']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['email']['field'] = 'email';
  $handler->display->display_options['fields']['email']['relationship'] = 'fromcid';
  $handler->display->display_options['fields']['email']['label'] = t('From');
  /* Field: Elobbyist Contact: Name */
  $handler->display->display_options['fields']['recipient_name']['id'] = 'recipient_name';
  $handler->display->display_options['fields']['recipient_name']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['recipient_name']['field'] = 'name';
  $handler->display->display_options['fields']['recipient_name']['relationship'] = 'tocid';
  $handler->display->display_options['fields']['recipient_name']['label'] = t('Recipient');
  /* Field: Elobbyist Contact: Email */
  $handler->display->display_options['fields']['recipient_email']['id'] = 'recipient_email';
  $handler->display->display_options['fields']['recipient_email']['table'] = 'elobbyist_contact';
  $handler->display->display_options['fields']['recipient_email']['field'] = 'email';
  $handler->display->display_options['fields']['recipient_email']['relationship'] = 'tocid';
  $handler->display->display_options['fields']['recipient_email']['label'] = t('To');
  /* Field: Elobbyist Template: Subject */
  $handler->display->display_options['fields']['subject']['id'] = 'subject';
  $handler->display->display_options['fields']['subject']['table'] = 'elobbyist_template';
  $handler->display->display_options['fields']['subject']['field'] = 'subject';
  $handler->display->display_options['fields']['subject']['relationship'] = 'tid';
  
  /* Field: Elobbyist Activity: View */
  $handler->display->display_options['fields']['view_link']['id'] = 'view_link';
  $handler->display->display_options['fields']['view_link']['table'] = 'elobbyist_activity';
  $handler->display->display_options['fields']['view_link']['field'] = 'view_link';
  $handler->display->display_options['fields']['view_link']['label'] = t('View Email');
  /* Field: Elobbyist Activity: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'elobbyist_activity';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = t('Sent');
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'Y-m-d G:i';

  /* Field: Elobbyist Activity: Mailing id */
  $handler->display->display_options['fields']['mid']['id'] = 'mid';
  $handler->display->display_options['fields']['mid']['table'] = 'elobbyist_activity';
  $handler->display->display_options['fields']['mid']['field'] = 'mid';
  $handler->display->display_options['fields']['mid']['exclude'] = TRUE;
  /* Field: Elobbyist Activity: Template id */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'elobbyist_activity';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  
  /* Contextual filter: Elobbyist Group: Group id */
  $handler->display->display_options['arguments']['mid']['id'] = 'mid';
  $handler->display->display_options['arguments']['mid']['table'] = 'elobbyist_activity';
  $handler->display->display_options['arguments']['mid']['field'] = 'mid';
  $handler->display->display_options['arguments']['mid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['mid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['mid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['mid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['mid']['summary_options']['items_per_page'] = $items_per_page;

  /* Filter criterion: Global: Combine fields filter */
  $handler->display->display_options['filters']['combine']['id'] = 'combine';
  $handler->display->display_options['filters']['combine']['table'] = 'views';
  $handler->display->display_options['filters']['combine']['field'] = 'combine';
  $handler->display->display_options['filters']['combine']['operator'] = 'contains';
  $handler->display->display_options['filters']['combine']['exposed'] = TRUE;
  $handler->display->display_options['filters']['combine']['expose']['operator_id'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['label'] = t('Search');
  $handler->display->display_options['filters']['combine']['expose']['operator'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['identifier'] = 'act_fields';
  $handler->display->display_options['filters']['combine']['fields'] = array(
    'name' => 'name',
    'email' => 'email',
    'recipient_name' => 'recipient_name',
    'recipient_email' => 'recipient_email',
    'subject' => 'subject',
  );
  $handler->display->display_options['path'] =  'elobbyist/mailing/%/emails';
    
  $views[$view->name] = $view;


  return $views;
}
