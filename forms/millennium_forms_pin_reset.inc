<?php

function millennium_forms_pin_reset_form() {
  $form['code'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Your Library Barcode'),
    '#description' => t('Enter your entire library card number.'
                         . ' Your account must be current and contain'
                         . ' a valid email address.'),
    '#required'    => TRUE
  );

  $form['notice'] = array(
    '#type'   => 'item',
    '#markup' => variable_get('millennium_forms_pin_reset_notice',
                              'If you cannot reset your PIN online, '
                              . 'please visit a local branch for assistance.')
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Send')
  );

  return $form;
}


function millennium_forms_pin_reset_form_submit($form, &$form_state) {
  $api = new WebPAC();
  
  $result = $api->pin_reset($form_state['values']['code']);
  
  if ($result->success)
    drupal_set_message(variable_get('millennium_forms_pin_reset_submitted',
                                    'Your PIN reset request was submitted.'));
  else
    switch ($result->error) {
      case 'pin reset failed':
        drupal_set_message('PIN Reset Failed: Please ensure your barcode was entered correctly, '
                           . 'and that the account is current and contains a valid email address.',
                           'error');
        break;
      default:
        drupal_set_message('An internal API error has occurred.', 'error');
        watchdog('WATCHDOG_ERROR', 'millennium_forms: ' . print_r($result, true));
    }
}
