<?php

/* * *****************
 * Theme functions *
 * ***************** */

/**
 * Default formatter for Streaming Media field
 */
function theme_streamingmediafield_formatter_default($element) {
  return $element['#item']['stream_name'];
}

/**
 * Streaming Media Widget Theme
 */
function theme_streaming_media($element) {
  return $element['#children'];
}