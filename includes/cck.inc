<?php

/* * ***************
 * CCK functions *
 * *************** */

/**
 * Implementation of hook_field_info().
 */
function streamingmediafield_field_info() {
  return array(
      'streaming_media' => array(
          'label' => t('Streaming Media'),
          'description' => t('Live or on-demand streaming media'),
      ),
  );
}

/**
 * Implementation of hook_formatter_info().
 */
function streamingmediafield_field_formatter_info() {
  return array(
      'default' => array(
          'label' => t('Default'),
          'field types' => array('streaming_media'),
          'multiple values' => CONTENT_HANDLE_CORE,
      ),
  );
}

/**
 * Implementation of hook_field_settings().
 */
function streamingmediafield_field_settings($op, $field) {
  switch ($op) {
    case 'form':
      $form = array();
      $type = content_types($field['type_name']);
      $fields = $type['fields'];
      foreach ($fields as $f) {
        if ($f['type'] == 'filefield' && $f['widget']['module'] == 'imagefield') {
          $imagefield_options[$f['field_name']] = $f['widget']['label'];
        }
      }

      if (!empty($imagefield_options)) {
        $form['still_image_field'] = array(
            '#type' => 'select',
            '#title' => t('Still Image Field'),
            '#options' => $imagefield_options,
            '#default_value' => $field['still_image_field'],
        );
      }
      return $form;
    case 'save':
      return array('still_image_field');
    case 'database columns':
      $columns['stream_name'] = array('type' => 'varchar', 'length' => 255, 'not null' => TRUE);
      $columns['stream_path'] = array('type' => 'varchar', 'length' => 255, 'not null' => FALSE);
      $columns['is_live'] = array('type' => 'int', 'size' => 'tiny', 'not null' => TRUE);
      $columns['progressive_download'] = array('type' => 'int', 'size' => 'tiny', 'not null' => TRUE);
      return $columns;
  }
}

/**
 * Generate a default value for streaming medai fields.
 */
function streamingmediafield_default_value(&$form, &$form_state, $field, $delta = 0) {
  $default_value = array();

  $default_value['is_live'] = 0;
  $default_value['progressive_download'] = 0;
  $default_value['stream_name'] = '';
  $default_value['stream_path'] = '';

  return array($default_value);
}

/**
 * Implementation of hook_content_is_empty
 */
function streamingmediafield_content_is_empty($item, $field) {
  return empty($item['stream_name']);
}

/**
 * Implementation of hook_widget_info().
 */
function streamingmediafield_widget_info() {
  $info = array();

  $info['streaming_media'] = array(
      'label' => t('Streaming media config'),
      'field types' => array('streaming_media'),
      'multiple values' => CONTENT_HANDLE_CORE,
      'callbacks' => array(
          'default value' => CONTENT_CALLBACK_CUSTOM,
      ),
  );

  return $info;
}

/**
 * Implementation of hook_widget()
 */
function streamingmediafield_widget(&$form, &$form_state, $field, $items, $delta = 0) {
  $stream_type = ($field['widget']['stream_type']) ? $field['widget']['stream_type'] : 'choose';
  $progressive_download_default = ($field['widget']['progressive_download_default']) ? $field['widget']['progressive_download_default'] : FALSE;

  $element = array('#tree' => TRUE, '#type' => 'fieldset', '#title' => $field['widget']['label'], '#description' => $field['widget']['description'], '#collapsible' => $field['widget']['collapsible']);

  if ($stream_type == 'choose') {
    $element['is_live'] = array(
        '#type' => 'radios',
        '#title' => t('Stream Type'),
        '#options' => array(
            1 => t('Live Stream'),
            0 => t('On Demand Stream'),
        ),
        '#description' => t('A live stream plays from a live video feed on the server. An on demand stream is stored on the server and can be played any time.'),
        '#default_value' => $items[$delta]['is_live'],
    );
  }
  else {
    $element['is_live'] = array('#type' => 'hidden', '#value' => ($stream_type == 'live') ? 1 : 0);
  }

  if ($stream_type == 'choose' || $stream_type == 'ondemand') {
    $element['stream_path'] = array(
        '#type' => 'textfield',
        '#title' => t('Stream Path'),
        '#description' => t('The directory on the streaming server where this stream can be found'),
        '#default_value' => $items[$delta]['stream_path'],
    );
    $element['progressive_download'] = array(
        '#type' => 'checkbox',
        '#title' => t('Use progressive download instead of streaming'),
        '#description' => t('If checked, the player will use progressive download technology rather than streaming.'),
        '#default_value' => ($form['nid']['#value']) ? $items[$delta]['progressive_download'] : $progressive_download_default,
    );

    if (module_exists('ctools')) {
      $field_key = sprintf('%s[%d][is_live]', $field['field_name'], $delta);
      ctools_include('dependent');
      $element['stream_path']['#process'] = array('ctools_dependent_process');
      $element['stream_path']['#dependency'] = array('radio:' . $field_key => array(0));
      $element['progressive_download']['#process'] = array('ctools_dependent_process');
      $element['progressive_download']['#dependency'] = array('radio:' . $field_key => array(0));
    }
  }
  else {
    // progressive download should always be FALSE for live streams
    $element['progressive_download'] = array('#type' => 'hidden', '#value' => 0);
  }

  $element['stream_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Stream Name'),
      '#description' => t('The name of the media stream'),
      '#default_value' => $items[$delta]['stream_name'],
  );

  return $element;
}

/**
 * Handle the parameters for a widget.
 *
 * @param $op
 *   The operation to be performed. Possible values:
 *   - "form": Display the widget settings form.
 *   - "validate": Check the widget settings form for errors.
 *   - "save": Declare which pieces of information to save back to the database.
 * @param $widget
 *   The widget on which the operation is to be performed.
 * @return
 *   This varies depending on the operation.
 *   - The "form" operation should return an array of form elements to add to
 *     the settings page.
 *   - The "validate" operation has no return value. Use form_set_error().
 *   - The "save" operation should return an array of names of form elements to
 *     be saved in the database.
 */
function streamingmediafield_widget_settings($op, $widget) {
  switch ($op) {
    case 'form':
      $form = array();
      $form['stream_type'] = array(
          '#type' => 'select',
          '#title' => t('Stream Type'),
          '#options' => array(
              'live' => t('Live Stream'),
              'ondemand' => t('On Demand Stream'),
              'choose' => t('Allow User to Choose Per Node'),
          ),
          '#default_value' => ($widget['stream_type']) ? $widget['stream_type'] : 'choose',
      );
      $form['collapsible'] = array(
          '#type' => 'checkbox',
          '#title' => t('Allow user to collapse this field'),
          '#default_value' => isset($widget['collapsible']) ? $widget['collapsible'] : TRUE,
      );

      $form['progressive_download_default'] = array(
          '#type' => 'checkbox',
          '#title' => t('Select progressive download by default for on-demand content'),
          '#default_value' => isset($widget['progressive_download_default']) ? $widget['progressive_download_default'] : FALSE,
      );

      return $form;
    case 'save':
      return array('stream_type', 'collapsible', 'progressive_download_default');
  }
}
