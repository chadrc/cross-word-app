<?hh
require_once("../src/elements/pages/base-page.hh");
require_once("../src/elements/content.hh");

function PuzzleSolve() {
  $data = fetchPost();
  $response = Map {};
  $response['message'] = "Submitted";
  return json_encode($response);
}
