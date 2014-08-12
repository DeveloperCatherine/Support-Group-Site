<?php
// $Id: advanced_menu_core.admin.inc $

/**
 * @file
 * Administrative page callbaks for advanced_menu_core module.
 */

/**
 * Menu callback which shows an overview page of all the custom menus and their descriptions.
 */
function advanced_menu_core_overview_page() {
  global $user;
  // User #1 has all privileges as does the 'administer menu' permission
  if ($user->uid == 1 || user_access('administer menu')) {
    $result = db_query("SELECT * FROM {menu_custom} ORDER BY title");
  }
  else {
    if (module_exists('advanced_menu_menu_admin')) {
      //Build the SELECT conditions based on the current user roles
      $select_condition = ' AND (';
      foreach ($user->roles as $key => $value) {
        if ($select_condition!=' AND (') { $select_condition .= ' OR '; }
        $select_condition .= 'roles.rid = ' . $key;
      }
      if ($select_condition != ' AND (') {
        $result = db_query("SELECT * FROM {menu_custom} RIGHT JOIN {advanced_menu_menu_admin_roles} roles ON (menu_custom.menu_name = roles.menu_name " . $select_condition . ")) ORDER BY title");
      }
    }
  }
  $content = array();
  while ($menu = db_fetch_array($result)) {
    $menu['href'] = 'admin/build/menu-customize/'. $menu['menu_name'];
    $menu['localized_options'] = array();
    $content[] = $menu;
  }
  return theme('admin_block_content', $content);
}

