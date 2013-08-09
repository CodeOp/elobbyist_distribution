<?php

class ELobbyistEmailProviderController extends EntityAPIControllerExportable {
  public function create(array $values = array()) {
    return (object)array_merge(array(
      'providerid' => NULL,
      'name' => NULL,
      'smtphost' => NULL,
      'smtpport' => variable_get('elobbyist_smtp_port_default', ELOBBYIST_DEFAULT_SMTP_PORT),
    ), $values);
  }
  
  public function save($entity) {
    $transaction = db_transaction();
    
    try {
      global $user;
      
      // Determine if we will be inserting a new record
      $entity->is_new = empty($entity->providerid);
            
      // Give modules the opportunity to prepare field data for
      // saving.
      field_attach_presave('elobbyist_email_provider', $entity);
            
      // If this is a new record...
      if ($entity->is_new) {
        // Save the new record.
        drupal_write_record('elobbyist_email_provider', $entity);
                
        $op = 'insert';
      }
      else {
        // Save the updated record
        drupal_write_record('elobbyist_email_provider', $entity, 'providerid');
        
        $op = 'update';
      }
            
      // Save fields.
      $function = 'field_attach_' . $op;
      $function('elobbyist_email_provider', $entity);
      
      module_invoke_all('entity_' . $op, $entity, 'elobbyist_email_provider');
     
      //clear internal properties
      unset($entity->is_new);
      
      // Ignore slave server temporarily to give time for the saved
      // order to be propagated to the slave.
      db_ignore_slave();

      return $entity;
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('elobbyist_email_provider', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }
  
  public function delete($ids) {
    if (empty($ids)) {
      return TRUE;
    }
      
    $entities = $this->load($ids, array());
    $transaction = db_transaction();
    
    try
    {
      db_delete('elobbyist_email_provider')
        ->condition('providerid', $ids, 'IN')
        ->execute();
      
      elobbyist_foreign_key_cascade_unset('elobbyist_contact', 'providerid', $ids);

      foreach($entities as $entity) {        
        field_attach_delete('elobbyist_email_provider', $entity);

        module_invoke_all('entity_delete', $entity, 'elobbyist_email_provider');
      }

      db_ignore_slave();
    }
    catch(Exception $e) {
      $transaction->rollback();
      watchdog_exception('elobbyist_email_provider', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
    
    return TRUE;
  }
  
}