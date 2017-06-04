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

function requestUri(): string {
  return $_SERVER['REQUEST_URI'] . "";
}
