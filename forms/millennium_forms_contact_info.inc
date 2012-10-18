<?php

function millennium_forms_contact_info_form() {
  $form['code'] = array(
    '#type'     => 'textfield',
    '#title'    => t('Enter your entire Multnomah County Library card number'),
    '#required' => TRUE
  );

  $form['pin'] = array(
    '#type'     => 'password',
    '#title'    => t('Enter your current PIN'),
    '#required' => TRUE
  );

  $form['address_line_1'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Address Line 1'),
    '#required'    => TRUE
  );

  $form['address_line_2'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Address Line 2'),
    '#required'    => TRUE
  );

  $form['telephone'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Telephone'),
    '#required'    => TRUE
  );

  $form['email'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Email'),
    '#required'    => TRUE
  );

  $form['location_code'] = array(
    '#type'         => 'select',
    '#title'        => t('Home Library'),
    '#description'  => t('Choose the library you usually visit and where you pick up your holds.'),
    '#options'      => array(
      '--' => '(Choose a Pickup Location)',
      'al' => 'Albina Library',
      'be' => 'Belmont Library',
      'ca' => 'Capitol Hill Library',
      'xx' => 'Central Library',
      'fv' => 'Fairview Library',
      'gh' => 'Gregory Heights Library',
      'gs' => 'Gresham Library',
      'hl' => 'Hillsdale Library',
      'hg' => 'Holgate Library',
      'hw' => 'Hollywood Library',
      'ke' => 'Kenton Library',
      'mi' => 'Midland Library',
      'np' => 'North Portland Library',
      'nw' => 'Northwest Library',
      'ro' => 'Rockwood Library',
      'sl' => 'Sellwood Library',
      'sj' => 'St. Johns Library',
      'tr' => 'Troutdale Library',
      'wo' => 'Woodstock Library',
      'lo' => 'Libary Outreach Services',
      'ad' => 'Admin'
    )
  );
                              
  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save')
  );

  return $form;
}


function millennium_forms_contact_info_form_validate($form, &$form_state) {
  if ($form_state['values']['location_code'] === '--')
    form_set_error('location_code', t('Please select a Home Library from the dropdown menu.'));
}


function millennium_forms_contact_info_form_submit($form, &$form_state) {
  $api = new WebPAC();
  $result = $api->modify_contact_info($form_state['values']['code'],
                                      $form_state['values']['pin'],
                                      $form_state['values']['address_line_1'],
                                      $form_state['values']['address_line_2'],
                                      $form_state['values']['email'],
                                      $form_state['values']['location_code'],
                                      $form_state['values']['telephone']
                                     );

  if ($result->success)
    drupal_set_message('Your contact info has been updated.');
  else
    switch ($result->error) {
      case 'login failed':
        drupal_set_message('Authentication failed:  Please ensure your barcode and PIN are correct.', 'error');
        break;
      default:
        drupal_set_message('An internal API error has occurred.', 'error');
        watchdog('WATCHDOG_ERROR', 'millennium_forms: ' . print_r($result, true));
    }
}

