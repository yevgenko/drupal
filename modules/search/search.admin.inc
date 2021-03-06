<?php
// $Id: search.admin.inc,v 1.10 2009-08-24 22:03:01 webchick Exp $

/**
 * @file
 * Admin page callbacks for the search module.
 */

/**
 * Menu callback: confirm wiping of the index.
 */
function search_reindex_confirm() {
  return confirm_form(array(), t('Are you sure you want to re-index the site?'),
                  'admin/config/search/settings', t(' The search index is not cleared but systematically updated to reflect the new settings. Searching will continue to work but new content won\'t be indexed until all existing content has been re-indexed. This action cannot be undone.'), t('Re-index site'), t('Cancel'));
}

/**
 * Handler for wipe confirmation
 */
function search_reindex_confirm_submit(&$form, &$form_state) {
  if ($form['confirm']) {
    search_reindex();
    drupal_set_message(t('The index will be rebuilt.'));
    $form_state['redirect'] = 'admin/config/search/settings';
    return;
  }
}

/**
 * Menu callback; displays the search module settings page.
 *
 * @ingroup forms
 * @see system_settings_form()
 * @see search_admin_settings_submit()
 * @see search_admin_reindex_submit()
 */
function search_admin_settings() {
  // Collect some stats
  $remaining = 0;
  $total = 0;
  foreach (module_implements('search') as $module) {
    $function = $module . '_search';
    $status = $function('status');
    $remaining += $status['remaining'];
    $total += $status['total'];
  }
  $count = format_plural($remaining, 'There is 1 item left to index.', 'There are @count items left to index.');
  $percentage = ((int)min(100, 100 * ($total - $remaining) / max(1, $total))) . '%';
  $status = '<p><strong>' . t('%percentage of the site has been indexed.', array('%percentage' => $percentage)) . ' ' . $count . '</strong></p>';
  $form['status'] = array('#type' => 'fieldset', '#title' => t('Indexing status'));
  $form['status']['status'] = array('#markup' => $status);
  $form['status']['wipe'] = array('#type' => 'submit', '#value' => t('Re-index site'), '#submit' => array('search_admin_reindex_submit'));

  $items = drupal_map_assoc(array(10, 20, 50, 100, 200, 500));

  // Indexing throttle:
  $form['indexing_throttle'] = array(
    '#type' => 'fieldset',
    '#title' => t('Indexing throttle')
  );
  $form['indexing_throttle']['search_cron_limit'] = array(
    '#type' => 'select',
    '#title' => t('Number of items to index per cron run'),
    '#default_value' => 100,
    '#options' => $items,
    '#description' => t('The maximum number of items indexed in each pass of a <a href="@cron">cron maintenance task</a>. If necessary, reduce the number of items to prevent timeouts and memory errors while indexing.', array('@cron' => url('admin/reports/status')))
  );
  // Indexing settings:
  $form['indexing_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Indexing settings')
  );
  $form['indexing_settings']['info'] = array(
    '#markup' => t('<p><em>Changing the settings below will cause the site index to be rebuilt. The search index is not cleared but systematically updated to reflect the new settings. Searching will continue to work but new content won\'t be indexed until all existing content has been re-indexed.</em></p><p><em>The default settings should be appropriate for the majority of sites.</em></p>')
  );
  $form['indexing_settings']['minimum_word_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum word length to index'),
    '#default_value' => 3,
    '#size' => 5,
    '#maxlength' => 3,
    '#description' => t('The number of characters a word has to be to be indexed. A lower setting means better search result ranking, but also a larger database. Each search query must contain at least one keyword that is this size (or longer).')
  );
  $form['indexing_settings']['overlap_cjk'] = array(
    '#type' => 'checkbox',
    '#title' => t('Simple CJK handling'),
    '#default_value' => TRUE,
    '#description' => t('Whether to apply a simple Chinese/Japanese/Korean tokenizer based on overlapping sequences. Turn this off if you want to use an external preprocessor for this instead. Does not affect other languages.')
  );

  $form['#submit'][] = 'search_admin_settings_submit';

  // Per module settings
  $form = array_merge($form, module_invoke_all('search', 'admin'));
  return system_settings_form($form, TRUE);
}

/**
 * Submit callback.
 */
function search_admin_settings_submit($form, &$form_state) {
  // If these settings change, the index needs to be rebuilt.
  if ((variable_get('minimum_word_size', 3) != $form_state['values']['minimum_word_size']) ||
      (variable_get('overlap_cjk', TRUE) != $form_state['values']['overlap_cjk'])) {
    drupal_set_message(t('The index will be rebuilt.'));
    search_reindex();
  }
}

/**
 * Submit callback.
 */
function search_admin_reindex_submit($form, &$form_state) {
  // send the user to the confirmation page
  $form_state['redirect'] = 'admin/config/search/settings/reindex';
}