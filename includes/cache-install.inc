<?php
// $Id: cache-install.inc,v 1.3 2009-07-28 12:13:46 dries Exp $

/**
 * A stub cache implementation to be used during the installation
 * process when database access is not yet available. Because Drupal's
 * caching system never requires that cached data be present, these
 * stub functions can short-circuit the process and sidestep the
 * need for any persistent storage. Obviously, using this cache
 * implementation during normal operations would have a negative impact
 * on performance.
 */

function cache_get($key, $table = 'cache') {
  return FALSE;
}

function cache_get_multiple(array &$cids, $bin = 'cache') {
  return array();
}

function cache_set($cid, $data, $table = 'cache', $expire = CACHE_PERMANENT, $headers = NULL) {
  return;
}

function cache_clear_all($cid = NULL, $table = NULL, $wildcard = FALSE) {
  return;
}
