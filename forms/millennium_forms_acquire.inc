<?php


function millennium_forms_acquire_form() {
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

  $form['author'] = array(
    '#type'     => 'textfield',
    '#title'    => t('Author'),
    '#size'     => 45,
    '#required' => TRUE
  );

  $form['title'] = array(
    '#type'     => 'textfield',
    '#title'    => t('Title'),
    '#size'     => 45,
    '#required' => TRUE
  );

  $form['publisher'] = array(
    '#type'     => 'textfield',
    '#title'    => t('Publisher'),
    '#size'     => 45,
    '#required' => FALSE
  );

  $form['isbn'] = array(
    '#type'     => 'textfield',
    '#title'    => t('ISSN/ISBN'),
    '#size'     => 45,
    '#required' => FALSE
  );

  $form['type'] = array(
    '#type'     => 'radios',
    '#title'    => t('What kind of resource is this?'),
    '#size'     => 40,
    '#options'  => array(
      'book'        => t('Book'),
      'audiobook'   => t('Audiobook'),
      'Ebook'       => t('Ebook'),
      'large print' => t('Large Print'),
      'music'       => t('Music'),
      'dvd'         => t('DVD'),
      'zine'        => t('Zine'),
      'database'    => t('Database'),
      'other'       => t('Other')
    ),
    '#default_value' => 'other',
    '#required' => TRUE
  );

  $form['subject'] = array(
    '#type'     => 'textfield',
    '#title'    => t('What is this about (what subject)?'),
    '#size'     => 60,
    '#required' => FALSE
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Send')
  );

  return $form;
}


function millennium_forms_acquire_form_validate($form, &$form_state) {
}


function millennium_forms_acquire_form_submit($form, &$form_state) {
  $api = new WebPAC();
  

  $result = $api->acquire($form_state['values']['code'],
                          $form_state['values']['pin'],
                          $form_state['values']['author'],
                          $form_state['values']['title'],
                          $form_state['values']['publisher'],
                          $form_state['values']['isbn'],
                          $form_state['values']['type'],
                          $form_state['values']['subject']
                         );

  if ($result->success)
    drupal_set_message(variable_get('millennium_forms_acquire_submitted', 
                                    'Your suggestion has been submitted.')); 
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
