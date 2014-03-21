<?php

/**
 * @file
 *  Admin file for the module.
 */

function domain_menuedit_access_admin($domain) {
  $menus = menu_load_all();
  return drupal_get_form('domain_menuedit_access_admin_form', $domain, $menus);
}

/**
 * Form constructor for the admin page.
 */
function domain_menuedit_access_admin_form($form, &$form_state, $domain, $menus) {
  foreach ($menus as $menu) {
    $options[$menu['menu_name']] = $menu['title'];
  }
  $form['domain_menus'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Menus assigned to %domain_name', array('%domain_name' => $domain['sitename'])),
    '#options' => $options,
    '#default_value' => !empty($domain['menus']) ? $domain['menus'] : array(),
    '#description' => t('Please select the menus that are attached to this domain. The users which are also attached to this domain, and have the "Administer menus assigned to domains" permission, will be able to edit these menus.')
  );
  $form['domain_menus']['#tree'] = TRUE;
  $form['domain_id'] = array(
    '#type' => 'value',
    '#value' => $domain['domain_id'],
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

function domain_menuedit_access_admin_form_submit($form, &$form_state) {
  $values = array_filter($form_state['values']['domain_menus']);
  domain_menuedit_access_update_menus($form_state['values']['domain_id'], array_keys($values));
  drupal_set_message(t('The settings have been saved.'));
}
