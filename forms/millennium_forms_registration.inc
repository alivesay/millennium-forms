<?php


function millennium_forms_registration_form() {
  $form['your_name'] = array(
    '#type'     => 'fieldset',
    '#title'    => t('Your name')
  );

  $form['your_name']['first_name'] = array(
    '#type'     => 'textfield',
    '#title'    => t('First name'),
    '#required' => TRUE
  );

  $form['your_name']['middle_name'] = array(
    '#type'     => 'textfield',
    '#title'    => t('Middle name'),
    '#required' => FALSE
  );

  $form['your_name']['last_name'] = array(
    '#type'     => 'textfield',
    '#title'    => t('Last name'),
    '#required' => TRUE
  );

  $form['mailing_set'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Mailing address')
  );

  $form['mailing_set']['mailing_address'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Street / P.O. Box'),
    '#required'    => TRUE
  );

  $form['mailing_set']['mailing_city'] = array(
    '#type'        => 'textfield',
    '#title'       => t('City'),
    '#size'        => 40,
    '#required'    => TRUE
  );

  $form['mailing_set']['mailing_state'] = array(
    '#type'          => 'select',
    '#options'       => millennium_forms_registration_form_build_states(),
    '#default_value' => 'OR',
    '#title'         => t('State'),
    '#required'      => TRUE
  );

  $form['mailing_set']['mailing_zipcode'] = array(
    '#type'        => 'textfield',
    '#title'       => t('ZIP Code'),
    '#size'        => 10,
    '#required'    => TRUE
  );

  $form['street_set'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Street address (if differrent)')
  );

  $form['street_set']['street_address'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Street / P.O. Box (if different than above)'),
    '#required'    => FALSE
  );

  $form['street_set']['street_city'] = array(
    '#type'        => 'textfield',
    '#title'       => t('City'),
    '#size'        => 40,
    '#required'    => FALSE 
  );

  $form['street_set']['street_state'] = array(
    '#type'        => 'select',
    '#options'     => millennium_forms_registration_form_build_states(),
    '#default_value' => 'OR',
    '#title'       => t('State'),
    '#required'    => FALSE
  );

  $form['street_set']['street_zipcode'] = array(
    '#type'        => 'textfield',
    '#title'       => t('ZIP Code'),
    '#size'        => 10,
    '#required'    => FALSE 
  );

  $form['contact_information'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Additional Contact Information')
  );

  $form['contact_information']['email'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Email'),
    '#required'    => FALSE
  );

  $form['contact_information']['telephone'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Telephone'),
    '#description' => t('Please use the format XXX-XXX-XXXX.'),
    '#size'        => 12,
    '#required'    => FALSE
  );

  $form['contact_information']['birthdate'] = array(
    '#type'        => 'date',
    '#title'       => t('Birthdate'),
    '#required'    => TRUE
  );

  $form['disclaimer_set'] = array(
    '#type'  => 'fieldset',
    '#title' => 'Responsibilities'
  );

  $form['disclaimer_set']['disclaimer'] = array(
    '#type'  => 'item',
    '#markup' => variable_get('millennium_forms_register_disclaimer', 'No disclaimer set.')
  );

  $form['submit'] = array(
    '#type'   => 'submit',
    '#value'  => t('Save'),
    '#weight' => 100
  );

  return $form;
}


function millennium_forms_registration_form_validate($form, &$form_state) {
  if ($form_state['values']['mailing_state'] === '--')
    form_set_error('mailing_state', t('Please select a valid mailing address state.'));

  if ($form_state['values']['street_state'] === '--')
    form_set_error('street_state', t('Please select a valid street address state.'));

  if (!empty($form_state['values']['telephone']) &&
      !preg_match('/^\d{3}\-\d{3}\-\d{4}$/', $form_state['values']['telephone']))
    form_set_error('telephone', t('Please enter a telephone number in the format XXX-XXX-XXXX.'));
}


function millennium_forms_registration_form_submit($form, &$form_state) {
  $api = new WebPAC();
  
  $mailing_address_line_2 = $form_state['values']['mailing_city'] . ' '
                            . $form_state['values']['mailing_state'] . ' '
                            . $form_state['values']['mailing_zipcode'];

  $street_address_line_2 = $form_state['values']['street_city'] . ' '
                            . $form_state['values']['street_state'] . ' '
                            . $form_state['values']['street_zipcode'];

  $birthdate = str_pad($form_state['values']['birthdate']['month'], 2, '0', STR_PAD_LEFT)
               . str_pad($form_state['values']['birthdate']['day'], 2, '0', STR_PAD_LEFT)
               . $form_state['values']['birthdate']['year'];

  $result = $api->register($form_state['values']['first_name'],
                           $form_state['values']['middle_name'],
                           $form_state['values']['last_name'],
                           $form_state['values']['mailing_address'],
                           $mailing_address_line_2,
                           $form_state['values']['street_address'],
                           $street_address_line_2,
                           $form_state['values']['telephone'],
                           $form_state['values']['email'],
                           $birthdate
                          );

  if ($result->success)
    drupal_set_message(variable_get('millennium_forms_register_submitted', 
                                    'Your card registration has been submitted.')); 
  else
    switch ($result->error) {
      default:
        drupal_set_message('An internal API error has occurred.', 'error');
        watchdog('WATCHDOG_ERROR', 'millennium_forms: ' . print_r($result, true));
    }
}


function millennium_forms_registration_form_build_states() {
  return array(
    'OR' => 'Oregon',
    'WA' => 'Washington',
    'ID' => 'Idaho',
    '--' => '--------',
    'AL' => 'Alabama',
    'AK' => 'Alaska',
    'AZ' => 'Arizona',
    'AR' => 'Arkansas',
    'CA' => 'California',
    'CO' => 'Colorado',
    'CT' => 'Connecticut',
    'DE' => 'Delaware',
    'DC' => 'District of Columbia',
    'FL' => 'Florida',
    'GA' => 'Georgia',
    'HI' => 'Hawaii',
    'IL' => 'Illinois',
    'IN' => 'Indiana',
    'IA' => 'Iowa',
    'KS' => 'Kansas',
    'KY' => 'Kentucky',
    'LA' => 'Louisiana',
    'ME' => 'Maine',
    'MD' => 'Maryland',
    'MA' => 'Massachusetts',
    'MI' => 'Michigan',
    'MN' => 'Minnesota',
    'MS' => 'Mississippi',
    'MO' => 'Missouri',
    'MT' => 'Montana',
    'NE' => 'Nebraska',
    'NV' => 'Nevada',
    'NH' => 'New Hampshire',
    'NJ' => 'New Jersey',
    'NM' => 'New Mexico',
    'NY' => 'New York',
    'NC' => 'North Carolina',
    'ND' => 'North Dakota',
    'OH' => 'Ohio',
    'OK' => 'Oklahoma',
    'PA' => 'Pennsylvania',
    'RI' => 'Rhode Island',
    'SC' => 'South Carolina',
    'SD' => 'South Dakota',
    'TN' => 'Tennessee',
    'TX' => 'Texas',
    'UT' => 'Utah',
    'VT' => 'Vermont',
    'VA' => 'Virginia',
    'WV' => 'West Virginia',
    'WI' => 'Wisconsin',
    'WY' => 'Wyoming'
  );
}

