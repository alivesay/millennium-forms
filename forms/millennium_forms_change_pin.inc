<?php

function millennium_forms_change_pin_form() {
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

  $form['new_pin'] = array(
    '#type'        => 'password_confirm',
    '#title'       => t('New PIN'),
    '#required'    => TRUE,
    '#process'     => array('form_process_password_confirm', 'millennium_forms_process_password_confirm')
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save')
  );

  return $form;
}


function millennium_forms_change_pin_form_validate($form, &$form_state) {
  if ($form_state['values']['code'] === $form_state['values']['new_pin']) 
    form_set_error('new_pin_1', t('Your PIN must be different from your barcode.'));

  if (!ctype_alnum($form_state['values']['new_pin']))
    form_set_error('new_pin_2', t('PIN must not contain any special characters.'));

  $len = strlen($form_state['values']['new_pin']);
  if ($len < 4 || $len > 10)
    form_set_error('new_pin_3', t('PIN must be between between 4 and 10 characters.'));
}


function millennium_forms_change_pin_form_submit($form, &$form_state) {
  $api = new WebPAC();
  $result = $api->change_pin($form_state['values']['code'],
                             $form_state['values']['pin'],
                             $form_state['values']['new_pin']);

  if ($result->success)
    drupal_set_message('Your PIN was changed successfully.');
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


function millennium_forms_process_password_confirm($element) {
  global $user;

  $element['pass1'] =  array(
    '#type' => 'password',
    '#title' => t('PIN'),
    '#value' => empty($element['#value']) ? NULL : $element['#value']['pass1'],
    '#required' => $element['#required'],
    '#attributes' => array('class' => array('password-field')),
  );
  $element['pass2'] =  array(
    '#type' => 'password',
    '#title' => t('Confirm PIN'),
    '#value' => empty($element['#value']) ? NULL : $element['#value']['pass2'],
    '#required' => $element['#required'],
    '#attributes' => array('class' => array('password-confirm')),
  );
  $element['#element_validate'] = array('password_confirm_validate');
  $element['#tree'] = TRUE;

  if (isset($element['#size'])) {
    $element['pass1']['#size'] = $element['pass2']['#size'] = $element['#size'];
  }

  $js_settings = array(
    'password' => array(
      'strengthTitle' => t('PIN strength:'),
      'hasWeaknesses' => t('To make your PIN stronger:'),
      'tooShort' => t('Make it at least 8 characters'),
      'addLetters' => t('Add letters'),
      'addNumbers' => t('Add numbers'),
      'isInvalid' => t('PIN is not valid:'),
      'isLessThanFour' => t('Must be at least 4 characters'),
      'hasPunctuation' => t('Cannot contain special characters'),
      'sameAsUsername' => t('Make it different from your barcode'),
      'confirmSuccess' => t('yes'),
      'confirmFailure' => t('no'),
      'weak' => t('Weak'),
      'fair' => t('Fair'),
      'good' => t('Good'),
      'strong' => t('Strong'),
      'invalid' => t('Invalid'),
      'confirmTitle' => t('PINs match:'),
      'username' => ''
    ),
  );


  $element['#attached']['js'][] = drupal_get_path('module', 'millennium_forms') . '/user.js';
  // Ensure settings are only added once per page.
  static $already_added = FALSE;
  if (!$already_added) {
    $already_added = TRUE;
    $element['#attached']['js'][] = array('data' => $js_settings, 'type' => 'setting');
  }

  return $element;
}


