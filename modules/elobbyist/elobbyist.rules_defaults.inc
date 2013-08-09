<?php

function elobbyist_default_rules_configuration() {
  $configs = array();

  $rule = '{ "elobbyist_remove_contacts_from_group" : {
      "LABEL" : "Remove from group",
      "PLUGIN" : "action set",
      "REQUIRES" : [ "elobbyist" ],
      "USES VARIABLES" : { "contacts" : { "label" : "Contacts", "type" : "list\u003Centity\u003E" } },
      "ACTION SET" : [
        { "elobbyist_rule_remove_contacts_add_group" : { "PROVIDE" : { "grpid" : { "grpid" : "Group ID" } } } },
        { "elobbyist_rule_remove_contacts_add_rids" : { "PROVIDE" : { "rids" : { "rids" : "Relation IDs" } } } },
        { "elobbyist_rule_remove_contacts" : { "contacts" : [ "contacts" ], "grpid" : [ "grpid" ], "rids" : [ "rids" ] } }
      ]
    }
  }';
  $configs['elobbyist_remove_contacts_from_group'] = rules_import($rule);
  
  $rule = '{ "elobbyist_add_contacts_to_group" : {
      "LABEL" : "Add to group",
      "PLUGIN" : "action set",
      "REQUIRES" : [ "elobbyist" ],
      "USES VARIABLES" : {
        "contacts" : { "label" : "contacts", "type" : "list\u003Centity\u003E" },
        "grpid" : { "label" : "Group ID", "type" : "integer" }
      },
      "ACTION SET" : [
        { "elobbyist_rule_add_contacts" : { "contacts" : [ "contacts" ], "grpid" : [ "grpid" ] } }
      ]
    }
  }';
  
  $configs['elobbyist_add_contacts_to_group'] = rules_import($rule);
  
  return $configs;

}
