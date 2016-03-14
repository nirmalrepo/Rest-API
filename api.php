<?php

require_once('./Rest.inc');

class API extends REST {

    public $invalid_request = "";

    public function processApi() {
        $func = strtolower(trim(str_replace("/", "", $_SERVER['REQUEST_URI'])));
        if ((int) method_exists($this, $func) > 0) {
            $this->$func();
        }else{
            header('HTTP/1.0 404 Not Found');
            exit();
        }
    }

    public function seeboregrestinsert() {
        if ($this->get_request_method() == "POST") {

            $this->clean_insert(file_get_contents('php://input'));
        } else {
            header('HTTP/1.0 404 Not Found');
            exit();
        }
    }

}

$api = new API;
$api->processApi();
