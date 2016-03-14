<?php

/* File : Rest.inc.php
 * Author :Nirmal Fernando
 * Description :Simple RestfulAPI for adding new users to Website DB via Seebo device request
 */
include_once './class/validation.inc';
include_once './class/dbc.php';
require_once './libs/PHPMailer/PHPMailerAutoload.php';

class REST extends Validation {

    public function response($data) {
        header('Content-type: application/json');
        echo $data;
    }

    public function get_request_method() {
        return $_SERVER['REQUEST_METHOD'];
    }

    public function clean_insert($request_body) {
        $pnode = $this->check_parent_node($request_body);
        if ($pnode) {
            $xml = simplexml_load_string($request_body);
            $this->xml_data_processing($xml);
        } else {
            $this->response($this->json($this->response_message('xml_parse_error')));
        }
    }

    private function xml_data_processing($xml) {
        if (isset($xml->Name) && isset($xml->Email) && isset($xml->Mac)) {
            $name = (string) $xml->Name;
            $email = (string) $xml->Email;
            $mobile = (string) $xml->Mobile;
            $mac = (string) $xml->Mac;
            $region = (string) $xml->Region;
            if (empty($name) || empty($email) || empty($mac)) {
                echo $this->response($this->json($this->response_message('empty_error')));
            } else {
                $list_columns = array($name, $email, $mobile, $mac, $region);
                $this->main_insert($this->valid_email($email), $this->check_email_exists($email), $this->mac_address($mac), $list_columns);
//                if ($this->valid_mac($mac)) {
//                    $mac_new = strtoupper(preg_replace("/[^A-Za-z0-9 ]/", '', $mac));
//                } else {
//                    echo $this->response($this->json($this->response_message('mac_format_error')));
//                }
            }
        } else {
            $this->response($this->json($this->response_message('error')));
        }
    }

    public function json($data) {
        if (is_array($data)) {
            return json_encode($data);
        }
    }

    private function response_message($res, $user) {
        switch ($res) {
            case 'success':
                return array("status" => 1, "msg" => "Done User added!");
            case 'success_n':
                return array("status" => 1, "msg" => $user);
            case 'update_sucsess':
                return array("status" => 1, "msg" => "User Modified");
            case 'error':
                return array("status" => 0, "msg" => "Invalid Values Error!");
            case 'empty_error':
                return array("status" => 0, "msg" => "Values are empty!");
            case 'xml_parse_error':
                return array("status" => 0, "msg" => "Couldn't read the file!");
            case 'email_validation_error':
                return array("status" => 0, "msg" => "Invalid Email!");
            case 'email_exists_error':
                return array("status" => 0, "msg" => "Duplicate Email!");
            case 'main_insert_error':
                return array("status" => 0, "msg" => "Error Adding User!");
            case 'mac_format_error':
                return array("status" => 0, "msg" => "Invalid MAC address format!");
            case 'sub_insert_error':
                return array("status" => 1, "msg" => "Added.User is already in user list!");
        }
    }

    private function main_insert($valid_email, $check_email_exists, $mac_address, $list_columns) {
        if ($valid_email) {
            if ($check_email_exists === 0) {
                $this->insert_to_db($mac_address, $list_columns);
            } else {
                $this->response($this->json($this->response_message('email_exists_error')));
            }
        } else {
            $this->response($this->json($this->response_message('email_validation_error')));
        }
    }

    private function second_insert($list_columns) {
        $get_user = MainConfig::get_user_id($list_columns);
        $check_user = $this->check_user_list($get_user);
        if ($check_user === 0) {
            $query = MainConfig::sub_insert($get_user);
            if ($query) {
                $this->send($list_columns);
                $this->response($this->json($this->response_message('success')));
            } else {
                $this->response($this->json($this->response_message('sub_insert_error')));
            }
        }
    }

    private function insert_to_db($mac_address, $list_columns) {
        if ($mac_address) {
            $query = MainConfig::insert($list_columns);
            if ($query) {
                $this->second_insert($list_columns);
            } else {
                $this->response($this->json($this->response_message('main_insert_error')));
            }
        } else {
            $query = MainConfig::update($list_columns);
            if ($query) {
                $this->response($this->json($this->response_message('update_sucsess')));
            } else {
                $this->response($this->json($query));
            }
        }
    }

    private function send($list_columns) {
        date_default_timezone_set('Etc/UTC');
        $results = MainConfig::select_from_db("SELECT * FROM `k9Lz1_mail_bank`");
        $m = new PHPMailer;
        $m->isSMTP();
        $m->SMTPAuth = TRUE;
        $m->SMTPDebug = 1;
        $m->SMTPOptions = array
            (
            'ssl' => array
                (
                'verify_peer' => false,
                'verify_peer_name' => false,
                'allow_self_signed' => true
            )
        );
        $m->Host = 'smtp.gmail.com';
        $m->Username = $results[12];
        $m->Password = $results[13];
        $m->SMTPSecure = 'tls';
        $m->Port = '587';
        $m->From = 'nirmalfsn89@gmail.com';
        $m->FromName = 'Nirmal Fernando';
        $m->addAddress($list_columns[1], 'Susith');

        $m->msgHTML(file_get_contents('welcome.html'), dirname(__FILE__));
        $m->send();
    }

}

?>