<?php

/**
 * @file
 * Hook implementations
 */


/**
 * Implementation of hook_theme().
 */
function streamingmediafield_theme() {
  return array(
      'streaming_media' => array(
          'arguments' => array('element' => NULL),
          'file' => 'includes/theme.inc',
      ),
      'streamingmediafield_formatter_default' => array(
          'arguments' => array('element' => NULL),
          'file' => 'includes/theme.inc',
      ),
  );
}

/**
 * Implementation of hook_menu().
 */
function streamingmediafield_menu() {
  $menu['admin/settings/streamingmediafield'] = array(
      'title' => 'Streaming Media CCK Field',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('streamingmediafield_admin_settings_form'),
      'access arguments' => array('administer streamingmediafield'),
      'file' => 'includes/admin.inc',
  );

  return $menu;
}

/**
 * Implementation of hook_perm().
 */
function streamingmediafield_perm() {
  return array('administer streamingmediafield');
}

/**
 * Implementation of hook_views_api().
 *
 * UNCOMMENT THIS FUNCTION WHEN WE HAVE FINISHED DEVELOPING THE VIEWS HANDLERS.
 */
/*
function streamingmediafield_views_api() {
  return array(
    'api' => 2.0,
    'path' => drupal_get_path('module', 'streamingmediafield') . '/views',
  );
}
 */