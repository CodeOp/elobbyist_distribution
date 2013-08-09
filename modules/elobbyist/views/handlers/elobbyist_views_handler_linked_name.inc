<?php

/**
 * @file
 * Contains the linked name field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a record.
 * Definition terms:
 * - link_to_record default: Should this field have the checkbox "link to record" enabled by default.
 * - url: url with '%' as the view->base_field place holder
 *
 * @ingroup views_field_handlers
 */
class elobbyist_views_handler_linked_name extends views_handler_field {

  function init(&$view, &$options) {
    parent::init($view, $options);
    
    $table = '';
    if (isset($options['table']) && !empty($options['table'])) {
      $table = $options['table'];
    }
    else {
      $table = $view->base_table;
    }
    
    if (entity_access('update', $table)) {
      return;
    }
    elseif (entity_access('view', $table)) {
      if (isset($this->definition['readonly url'])) {
        $this->definition['url'] = $this->definition['readonly url'];
      }
      else
      {
        unset($this->definition['url']);
      }
    }
    else {
      unset($this->definition['url']);
    }
  }
  
  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_record'] = array('default' => isset($this->definition['link_to_record default']) ? $this->definition['link_to_record default'] : FALSE, 'bool' => TRUE);
    return $options;
  }

  /**
   * Provide link to node option
   */
  function options_form(&$form, &$form_state) {
    $form['link_to_record'] = array(
      '#title' => t('Link this field to the record edit form'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_record']),
    );

    parent::options_form($form, $form_state);
    
    
  }

  /**
   * Render whatever the data is as a link to the record.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_record'])) {
      $fieldval = NULL;
      
      if (isset($this->definition['id field']) && isset($this->view->field[$this->definition['id field']])) {
        $deffieldname = $this->view->field[$this->definition['id field']]->field_alias;
        $fieldname = NULL;
        if (!empty($this->table_alias )) {
          $fieldname = $this->table_alias . '_' . $this->definition['id field'];
          if (!isset($values->$fieldname)) {
            $fieldname = $deffieldname;
          }
        }
        else
        {
          $fieldname = $deffieldname;
        }
        
        $fieldval = $values->$fieldname;
      }
      else {
        $fieldval = $this->view->field[$this->view->base_field]->original_value;
      }
      if (!empty($data) && !empty($fieldval) && isset($this->definition['url'])) {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = str_replace('%', $fieldval, 
          $this->definition['url']);
      }
      else {
        $this->options['alter']['make_link'] = FALSE;
      }
    }
    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}
