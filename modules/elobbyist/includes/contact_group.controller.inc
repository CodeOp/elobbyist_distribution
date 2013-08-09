<?php

class ELobbyistContactGroupController extends EntityAPIControllerExportable {
  
  public function create(array $values = array()) {
    return (object)array_merge(array(
      'grpid' => NULL,
      'name' => NULL,
      'status' => ELOBBYIST_GROUP_STATUS_ACTIVE,
      'type' => NULL,
    ), $values);
  }
  
  public function save($entity) {
    $transaction = db_transaction();
    
    try {
      global $user;
      
      // Determine if we will be inserting a new record
      $entity->is_new = empty($entity->grpid);
            
      // Give modules the opportunity to prepare field data for
      // saving.
      field_attach_presave('elobbyist_group', $entity);
            
      // If this is a new record...
      if ($entity->is_new) {
        // Save the new record.
        drupal_write_record('elobbyist_group', $entity);
                
        $op = 'insert';
      }
      else {
        // Save the updated record
        drupal_write_record('elobbyist_group', $entity, 'grpid');
        
        $op = 'update';
      }
            
      // Save fields.
      $function = 'field_attach_' . $op;
      $function('elobbyist_group', $entity);
      
      module_invoke_all('entity_' . $op, $entity, 'elobbyist_group');
     
      //clear internal properties
      unset($entity->is_new);
      
      // Ignore slave server temporarily to give time for the saved
      // order to be propagated to the slave.
      db_ignore_slave();

      return $entity;
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('elobbyist_group', $e, NULL, WATCHDOG_ERROR);
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
      db_delete('elobbyist_group')
        ->condition('grpid', $ids, 'IN')
        ->execute();

      foreach($entities as $entity) {        
        field_attach_delete('elobbyist_group', $entity);

        module_invoke_all('entity_delete', $entity, 'elobbyist_group');
      }
      
      //unset foreign key of mailing addtogrpid
      elobbyist_foreign_key_cascade_unset('elobbyist_mailing', 'addtogrpid', $ids);

      db_ignore_slave();
    }
    catch(Exception $e) {
      $transaction->rollback();
      watchdog_exception('elobbyist_group', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
    
    return TRUE;
  }
  
}