<?php

/**
 * Implements hook_features_export().
 */
function domain_menuedit_access_features_export($data, &$export, $module_name = '') {
  // Add required dependencies.
  $export['dependencies']['features'] = 'features';
  $export['dependencies']['domain_menuedit_access'] = 'domain_menuedit_access';
  $list = domain_features_selection($data);
  foreach ($list as $machine_name) {
    $export['features']['domain_menuedit_access'][$machine_name] = $machine_name;
  }
}

/**
 * Implements hook_features_export_options().
 */
function domain_menuedit_access_features_export_options() {
  $options = array();

  if ($domains = domain_domains()) {
    foreach ($domains as $domain) {
      $options[$domain['machine_name']] = $domain['sitename'];
    }
  }
  return $options;
}

/**
 * Implements hook_features_export_render().
 */
function domain_menuedit_access_features_export_render($module, $data, $export = NULL) {
  $output = array();
  $output[] = '  $items = array(';
  foreach ($data as $domain_name) {
    if ($domain_id = domain_load_domain_id($domain_name)) {
      $output[] = "    '{$domain_name}' => " . features_var_export(domain_menuedit_access_get_menus_for_domain($domain_id), '    ') . ",";
    }
  }
  $output[] = '  );';
  $output[] = '  return $items;';
  $output = implode("\n", $output);
  return array('domain_menuedit_access_default' => $output);
}

/**
 * Implements hook_features_revert().
 */
function domain_menuedit_access_features_revert($module = NULL) {
  if (domain_features_load($module, 'domain_menuedit_access_default')) {
    $defaults = domain_features_load($module, 'domain_menuedit_access_default');
    foreach ($defaults as $domain_name => $menu_names) {
      if ($domain_id = domain_load_domain_id($domain_name)) {
        domain_menuedit_access_update_menus($domain_id, $menu_names);
      }
      else {
        drupal_set_message(t('Could not find domain !domain.', array('!domain' => $domain_name)), 'error');
      }
    }
  }
  else {
    drupal_set_message(t('Could not load default domain menuedit access settings.'), 'error');
    return FALSE;
  }

  return TRUE;
}

/**
 * Implements hook_features_rebuild().
 */
function domain_menuedit_access_features_rebuild($module) {
  return domain_menuedit_access_features_revert($module);
}
