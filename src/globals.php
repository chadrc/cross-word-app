<?php

function fetchGET(): array {
   return $_GET;
}

function getString(string $key): string {
  try {
    $value = $_GET[$key];
    return is_string($value) ? $value : null;
  } catch(Exception $e) {
    return null;
  }
}

function fetchPOST(): array {
   return $_POST;
}

function postString(string $key): string {
  try {
    $value = $_POST[$key];
    return is_string($value) ? $value : null;
  } catch(Exception $e) {
    return null;
  }
}

function postType(): string {
   if($_SERVER['REQUEST_METHOD'] === 'GET'){
      return "GET";
   } else {
      return "POST";
   }
}

function requestMethod() {
  $method = $_SERVER['REQUEST_METHOD'];
  return (string) $method;
}

function requestUri() {
  $uri = $_SERVER['REQUEST_URI'];
  return (string) $uri;
}

function fetchPOSTBody() {
  return file_get_contents('php://input');
}
