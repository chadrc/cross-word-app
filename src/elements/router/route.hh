<?hh
require_once("../src/elements/pages/base-page.hh");

class :route extends :x:element {
  attribute string match @required;
  children (:base-page);

  protected function render(): XHPRoot {
    $uri_parts = new Vector(explode("/", requestUri()));
    $path = Vector {};
    foreach ($uri_parts as $value) {
      if ($value !== "") {
        $path[] = $value;
      }
    }

    $get_data = new Map(fetchGET());
    $post_data = new Map(fetchPOST());
    $root = null;
    $first = $this->getChildren()[0];
    if ($first instanceof :base-page) {
      $first->set_request_info($path, $get_data, $post_data);
      return $first;
    }
    return <html></html>;
  }

  public function get_match(): string {
    return $this->:match;
  }
}
