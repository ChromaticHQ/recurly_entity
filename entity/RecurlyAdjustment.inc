<?php

class RecurlyAdjustment extends RecurlyEntity {

  /**
   * Override __construct to force an entity type.
   */
  public function __construct(array $values = array(), $entityType = 'recurly_adjustment') {
    // Adjustments are not modifiable by the user.
    $this->status = ENTITY_FIXED;

    // TODO: Can this be moved up to RecurlyEntity for everything with a UUID?
    // We need to map to a local ID if possible. This can happen if a plan has
    // been saved locally but our entity cache is empty.
    if (!isset($values['id'])) {
      $id = entity_get_controller('recurly_adjustment')
        ->entityId($values['uuid']);
      if ($id) {
        $values['id'] = $id;
      }
    }

    // Generate a title for this adjustment.
    if (!isset($values['title'])) {
      $values['title'] = substr($values['description'], 0, 255);
    }

    $this->setOwner($values);

    // Pop out the stub invoice into a UUID.
    if (isset($values['invoice'])) {
      recurly_client_initialize();
      $values['invoice'] = $values['invoice']->get()->uuid;
    }

    // Convert the created at date into a DATETIME.
    $values['created_at'] = $values['created_at']
      ->setTimezone(new DateTimeZone('UTC'))
      ->format('U');

    parent::__construct($values, $entityType);
  }

  /**
   * Overrides uri().
   */
  public function uri() {
    return array(
      'path' => 'recurly/adjustment/' . $this->id,
    );
  }
}