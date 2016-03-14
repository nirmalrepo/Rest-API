<?php

class MainConfig {

    const DB_SERVER = "localhost";
    const DB_USER = "rest_api";
    const DB_PASSWORD = "restapi";
    const DB = "see_test_new";

    private $db = NULL;

    /*
     *  Database connection 
     */

    public function dbConnect() {
        $this->db = mysql_connect(self::DB_SERVER, self::DB_USER, self::DB_PASSWORD);
        if ($this->db)
            mysql_select_db(self::DB, $this->db);
    }

    public static function closeDB() {
        mysql_close();
    }

    public function insert($list_columns) {
        MainConfig::dbConnect();
        $name = $list_columns[0];
        $email = $list_columns[1];
        $mobile = $list_columns[2];
        $mac = $list_columns[3];
        $region = $list_columns[4];
        $created_at = time();
        $key_user = md5(AUTH_KEY . $list_columns[1] . $created_at);
        $domain_name = substr(strrchr($email, "@"), 1);
        $sql = "INSERT INTO `k9Lz1_wysija_user` (`user_id`, `wpuser_id`, `email`, `firstname`, `keyuser`,`created_at`,`status`,`domain`,`cf_1`,`cf_3`,`cf_4`) VALUES (NULL, '0', '$email', '$name', '$key_user','$created_at','1','$domain_name','$mobile','$mac','$region');";
        $qur = mysql_query($sql);
        if ($qur) {
            return TRUE;
        } else {
            return mysql_error();
        }
    }

    public function sub_insert($user) {
        $sub_date = time();
        $sql = "INSERT INTO `k9Lz1_wysija_user_list` (`list_id`, `user_id`, `sub_date`, `unsub_date`) VALUES ('6', '$user', '$sub_date', '0');";
        $qur = mysql_query($sql);
        if ($qur) {
            return TRUE;
        } else {
            return mysql_error();
        }
        MainConfig::closeDB();
    }

    public function update($list_columns) {
        MainConfig::dbConnect();
        $name = $list_columns[0];
        $email = $list_columns[1];
        $mobile = $list_columns[2];
        $mac = $list_columns[3];
        $region = $list_columns[4];
        $domain_name = substr(strrchr($email, "@"), 1);
        $sql = "UPDATE `k9Lz1_wysija_user` SET email='$email',firstname='$name',domain='$domain_name',cf_1='$mobile',cf_4='$region' WHERE cf_3='$mac'";
        $qur = mysql_query($sql);
        if ($qur) {
            return TRUE;
        } else {
            return mysql_error();
        }
    }

    public function get_no_of_rows($sql) {
        MainConfig::dbConnect();
        $r_query = mysql_query($sql);
        if ($r_query) {
            $rowcount = mysql_num_rows($r_query);
            return $rowcount;
        } else {
            return mysql_error();
        }
    }

    public function get_user_id($list_columns) {
//        return $list_columns[1];
        $sql = "SELECT * FROM `k9Lz1_wysija_user` WHERE email LIKE '%" . $list_columns[1] . "%'";
        $user_id = MainConfig::select_from_db($sql);
        return $user_id[0];
    }

    public function select_from_db($sql) {
        MainConfig::dbConnect();
        $r_query = mysql_query($sql);
        if ($r_query) {
            while ($row = mysql_fetch_array($r_query)) {
                return $row;
            }
        } else {
            return 'FALSE';
        }
    }

}
