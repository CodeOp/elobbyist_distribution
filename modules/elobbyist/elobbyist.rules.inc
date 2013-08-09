<?php

/*
 * Implements hook_rules_action_info
 */
function elobbyist_rules_action_info() {
  return array(
    'elobbyist_rule_remove_contacts_add_group' => array(
      'label' => t('Add group variable from url'),
      'provides' => array(
        'grpid' => array(
          'type' => 'integer',
          'label' => t('Group ID'),
        ),
      ),
      'group' => t('Email Lobbyist'),
    ),
    'elobbyist_rule_remove_contacts_add_rids' => array(
      'label' => t('Add Relation ids from post values'),
      'provides' => array(
        'rids' => array(
          'type' => 'list<integer>',
          'label' => t('Relation IDs'),
        ),
      ),
      'group' => t('Email Lobbyist'),
    ),
    'elobbyist_rule_remove_contacts' => array(
      'label' => t('Remove Contacts from Group'),
      'parameter' => array(
        //must have first parameter as entity, to integrate with VBO
        'contacts' => array('type' => 'list<entity>', 'label' => t('Contacts')),
        'grpid' => array('type' => 'integer', 'label' => t('Group ID')),
        'rids' => array('type' => 'list<integer>', 'label' => t('Relation IDs')),
      ),
      'group' => t('Email Lobbyist'),
      'access callback' => 'elobbyist_rule_remove_contacts_access',
    ),
    'elobbyist_rule_add_contacts' => array(
      'label' => t('Add Contacts to Group'),
      'parameter' => array(
        //must have first parameter as entity, to integrate with VBO
        'contacts' => array('type' => 'list<entity>', 'label' => t('Contacts')),
        'grpid' => array('type' => 'integer', 'label' => t('Group Id')),
      ),
      'group' => t('Email Lobbyist'),
      'access callback' => 'elobbyist_rule_add_contacts_access',
    ),
  );
}

function elobbyist_rule_remove_contacts_add_group($args, $element) {
  //return the group id for group member paths
  return array('grpid' => arg(2));
}

function elobbyist_rule_remove_contacts_add_rids($args, $element) {
  global $_POST;
  if (isset($_POST['rids'])) {
    return array('rids' => unserialize(base64_decode($_POST['rids'])));
  }
}

/*
 * Callback for elobbyist_rule_remove_contact
 * 
 */
function elobbyist_rule_remove_contacts($contacts, $grpid, $rids) { 
  $cids = array_keys($contacts);
   
  foreach($cids as $cid) {
    relation_delete($rids[$cid]);
  }
}

/**
 * access callback for elobbyist_rule_remove_contact
 */
function elobbyist_rule_remove_contacts_access($type, $name) {
  return entity_access('update', 'elobbyist_contact');
}

function elobbyist_rule_add_contacts($contacts, $grpid) {  
  foreach($contacts as $cid => $contact) {
    if ($contact->type == 'subscriber') {
      elobbyist_relations_create('elobbyist_contact', $cid, 'elobbyist_group', $grpid,
            'elobbyist_subscriber');
    }
    else {
      elobbyist_relations_create('elobbyist_contact', $cid, 'elobbyist_group', $grpid,
            'elobbyist_recipient');
    }
  }
}

function elobbyist_rule_add_contacts_access($type, $name) {
  return entity_access('update', 'elobbyist_contact');
}