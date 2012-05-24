<?php

/**
 * @file
 * Provide views data for streamingmediafield.module.
 */
/**
 * @defgroup views_streamingmediafield_module streamingmediafield.module handlers
 *
 * Includes the handler for the streamingmediafield data column.
 * @{
 */

/**
 * Implementation of hook_views_data()
 */
function streamingmediafield_views_data() {
  $data = array();

  return $data;
}

/**
 * Implementation of hook_views_handlers().
 */
function streamingmediafield_views_handlers() {
  $handlers = array(
      'info' => array(
          'path' => drupal_get_path('module', 'streamingmediafield') . '/views',
      ),
      'handlers' => array(
          // field handlers
          'streamingmediafield_handler_field_data' => array(
              'parent' => 'views_handler_field_node',
          ),
          'streamingmediafield_handler_field_icon' => array(
              'parent' => 'views_handler_field',
          ),
      ),
  );

  return $handlers;
}

/**
 * @}
 */
