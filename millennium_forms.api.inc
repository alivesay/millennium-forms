<?php

class WebPAC {
   
  public $version = '1';
  public $api_url;


  public function __construct() {
    $this->api_url = variable_get('millennium_forms_webpac_api_url',
                                  'http://localhost');
  }


  protected function _request($resource, $method, $data) {
    $rtv = new stdClass();
    $rtv->success = FALSE;

    $url = $this->api_url . $resource;

    $options = array();

    // setup request
    switch($method) {
      case 'PUT':
        $options['method'] = 'PUT';
        $options['headers'] = array('content-type' => 'application/x-www-form-urlencoded');
        break;
      case 'GET':
        $options['headers'] = array('content-type' => 'application/json');
        break;
      default:
        break;
    }

    $options['data'] = $data;

    $rtv->url = $url;
    $rtv->response = drupal_http_request($url, $options);
    
    // process response
    switch ($rtv->response->code) {
      // good response codes
      case 200:
        
        if (!empty($rtv->response->data)) {
          $rtv->data = json_decode($rtv->response->data);
        
          if (is_null($rtv->data))
            $rtv->error = $this->json_error_string(json_last_error());
          else
            $rtv->success = TRUE;
        
        } else {
          $rtv->success = TRUE;
        }

        return $rtv;
        break;

      // bad response codes
      case 401:
        $rtv->error = 'login failed';
        break;

      case 400:
        if (!empty($rtv->response->data)) {
          $data = json_decode($rtv->response->data);
          if (!is_null($data) && property_exists($data, 'error'))
            $rtv->error = $data->error;
        }

        if (!isset($rtv->error))
            $rtv->error = '400 - unknown error';
        break;

      case 403:
      case 404:

        break;
 
      default:
        $rtv->error = 'unhandled http response: '.$rtv->response->code.' '.$rtv->response->error;
        break;
    }

    if (!isset($rtv->error))
      $rtv->error = 'unhandled error';
    
    watchdog('WATCHDOG_ERROR', 'Millennium Forms API error: ' . $rtv->error);

    return $rtv;
  }


  public function change_pin($code, $pin, $new_pin) {
    $data = array(
      'code'    => $code,
      'pin'     => $pin,
      'new_pin' => $new_pin
    );

    return $this->_request('/change_pin', 'PUT', json_encode($data));
  }


  public function modify_contact_info($code, $pin, $address_line_1, $address_line_2,
                                      $email, $location_code, $telephone) {
    $data = array(
      'code'  => $code,
      'pin'   => $pin,
      'address_line_1' => $address_line_1,
      'address_line_2' => $address_line_2,
      'email' => $email,
      'location_code' => $location_code, 
      'telephone' => $telephone
    );

    return $this->_request('/modify_contact_info', 'PUT', json_encode($data));
  } 


  public function register($first_name, $middle_name, $last_name,
                           $mailing_address_line_1, $mailing_address_line_2,
                           $street_address_line_1, $street_address_line_2,
                           $telephone, $email, $birthdate) {
    $data = array(
      'first_name'             => $first_name,
      'middle_name'            => $middle_name,
      'last_name'              => $last_name,
      'mailing_address_line_1' => $mailing_address_line_1,
      'mailing_address_line_2' => $mailing_address_line_2,
      'street_address_line_1'  => $street_address_line_1,
      'street_address_line_2'  => $street_address_line_2,
      'telephone'              => $telephone,
      'email'                  => $email,
      'birthdate'              => $birthdate
    );

    return $this->_request('/register', 'PUT', json_encode($data));
  }


  public function acquire($code, $pin, $author, $title, $publisher,
                          $isbn, $type, $subject) {
    $data = array(
      'code'  => $code,
      'pin'   => $pin,
      'author' => $author,
      'title' => $title,
      'publisher' => $publisher,
      'isbn' => $isbn, 
      'type' => $type,
      'subject' => $subject
    );

    return $this->_request('/acquire', 'PUT', json_encode($data));
  } 


  public function pin_reset($code) {
    $data = array('code'  => $code);

    return $this->_request('/pin_reset', 'PUT', json_encode($data));
  } 


  protected function json_error_string($errnum) {
    switch ($errnum) {
      case JSON_ERROR_NONE:
        return "JSON_ERROR_NONE";
      case JSON_ERROR_DEPTH:
        return "JSON_ERROR_DEPTH";
      case JSON_ERROR_STATE_MISMATCH:
        return "JSON_ERROR_STATE_MISMATCH";
      case JSON_ERROR_CTRL_CHAR:
        return "JSON_ERROR_CTRL_CHAR";
      case JSON_ERROR_SYNTAX:
        return "JSON_ERROR_SYNTAX";
      case JSON_ERROR_UTF8:
        return "JSON_ERROR_UTF8";
      default:
       return "UNKNOWN JSON ERROR";
    }
  }


}


