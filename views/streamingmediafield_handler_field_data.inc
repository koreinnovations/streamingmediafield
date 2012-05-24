<?php

/**
 * @file
 * streamingmediafield_handler_field_data.inc
 *
 * Provides a handler for displaying values within the serialized data column.
 */
class streamingmediafield_handler_field_data extends views_handler_field_node {

  function option_definition() {
    $options = parent::option_definition();
    $options['data_key'] = array('default' => 'stream_name');
    return $options;
  }

  function local_options() {
    $options = array(
        'stream_path' => t('Stream Path'),
        'stream_name' => t('Stream Name'),
    );
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = $this->local_options();

    $form['data_key'] = array(
        '#title' => t('Data key'),
        '#type' => 'radios',
        '#options' => $options,
        '#required' => TRUE,
        '#default_value' => $this->options['data_key'],
        '#description' => t('Which component of this field would you like to show?'),
        '#weight' => 4,
    );
  }

  function admin_summary() {
    // Display the data to be displayed.
    $info = $this->local_options();
    return isset($info[$this->options['data_key']]['title']) ? $info[$this->options['data_key']]['title'] : $this->options['data_key'];
  }

  function render($values) {
    $values = drupal_clone($values); // Prevent affecting the original.
    $data = unserialize($values->{$this->field_alias});
    dpm(array(
        'values' => $values,
        'data' => $data,
    ));
    $values->{$this->field_alias} = 'n/a'; //streamingmediafield_data_value($this->options['data_key'], $data[$this->options['data_key']]);
    // Copied from views_handler_field_node(). We just remove the call to
    // sanitize_value() from the original call, becaue our value has already
    // been cleaned by streamingmediafield_data_value().
    return $this->render_link($values->{$this->field_alias}, $values);
  }

}
