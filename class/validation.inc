<?php

include './class/dbc.php';

Class Validation {

    public function check_parent_node($request_body) {
        try {
            $sxe = new SimpleXMLElement($request_body);
            if ($sxe->getName() == 'Customer') {
                return TRUE;
            }
        } catch (Exception $e) {
            return FALSE;
        }
    }

    public function valid_email($email) {
        if (filter_var($email, FILTER_VALIDATE_EMAIL)) {
            return TRUE;
        }
    }

    public function check_email_exists($email) {
        $sql = "SELECT * FROM `k9Lz1_wysija_user` WHERE email LIKE '%" . $email . "%'";
        $val = MainConfig::get_no_of_rows($sql);
        return $val;
    }

    public function check_user_list($get_user) {
        $sql = "SELECT * FROM `k9Lz1_wysija_user_list` WHERE user_id LIKE '%" . $get_user . "%'";
        $val = MainConfig::get_no_of_rows($sql);
        return $val;
    }

    public function mac_address($mac) {
        $sql = "SELECT * FROM `k9Lz1_wysija_user` WHERE cf_3 LIKE '$mac'";
        $val = MainConfig::get_no_of_rows($sql);
        if ($val === 0) {
            return TRUE;
        }
    }

    public function valid_mac($mac) {
        if (preg_match('/^([a-fA-F0-9]{2}:){5}[a-fA-F0-9]{2}$/', $mac))
            return true;
        // 01-23-45-67-89-ab
        if (preg_match('/^([a-fA-F0-9]{2}\-){5}[a-fA-F0-9]{2}$/', $mac))
            return true;
        // 0123456789ab
        else if (preg_match('/^[a-fA-F0-9]{12}$/', $mac))
            return true;
        // 0123.4567.89ab
        else if (preg_match('/^([a-fA-F0-9]{4}\.){2}[a-fA-F0-9]{4}$/', $mac))
            return true;
        else
            return false;
    }

}
