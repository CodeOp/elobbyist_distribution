<?php

/**
 * @file
 * non-generic views handlers
 * 
 */

class elobbyist_views_handler_filter_mailing extends views_handler_filter_many_to_one {
  function get_value_options() {
    
    $query = db_select('elobbyist_mailing', 'm');
    $query->fields ('m', array (
                  'mid',
                  'name',
                )
            );
    $results = $query->execute();

    //go through resultset
    $rows = array();
    if ($results) {
      foreach ($results as $row) {
        $rows[$row->mid] = $row->name;
      }
    }
    
    $this->value_options = $rows;
  }
}

class elobbyist_views_handler_filter_template extends views_handler_filter_many_to_one {
  function get_value_options() {
    
    $query = db_select('elobbyist_template', 't');
    $query->fields ('t', array (
                  'tid',
                  'name',
                )
            );
    $results = $query->execute();

    //go through resultset
    $rows = array();
    if ($results) {
      foreach ($results as $row) {
        $rows[$row->tid] = $row->name;
      }
    }
    
    $this->value_options = $rows;
  }
}

class elobbyist_views_handler_filter_contact extends views_handler_filter_many_to_one {
  function get_value_options() {
    
    $query = db_select('elobbyist_contact', 'c');
    $query = $query->fields ('c', array (
                  'cid',
                  'name',
                )
            );
    
    if (isset($this->definition['contact type'])) {
      $query = $query->condition('type', $this->definition['contact type']);
    }
    $results = $query->execute();

    //go through resultset
    $rows = array();
    if ($results) {
      foreach ($results as $row) {
        $rows[$row->cid] = $row->name;
      }
    }
    
    $this->value_options = $rows;
  }
}

class elobbyist_views_handler_filter_email_provider extends views_handler_filter_many_to_one {
  function get_value_options() {
    
    $query = db_select('elobbyist_email_provider', 'ep');
    $query->fields ('ep', array (
                  'providerid',
                  'name',
                )
            );
    $results = $query->execute();

    //go through resultset
    $rows = array();
    if ($results) {
      foreach ($results as $row) {
        $rows[$row->providerid] = $row->name;
      }
    }
    
    $this->value_options = $rows;
  }
}

class elobbyist_views_handler_filter_group extends views_handler_filter_many_to_one {
  function get_value_options() {
    
    $query = db_select('elobbyist_group', 'g');
    $query = $query->fields ('g', array (
                  'grpid',
                  'name',
                )
            );
    
    if (isset($this->definition['group type'])) {
      $query = $query->condition('type', $this->definition['group type']);
    }
    $results = $query->execute();

    //go through resultset
    $rows = array();
    if ($results) {
      foreach ($results as $row) {
        $rows[$row->grpid] = $row->name;
      }
    }
    
    $this->value_options = $rows;
  }
}