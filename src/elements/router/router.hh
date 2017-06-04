<?hh
require_once("../src/elements/router/route.hh");

class :router extends :x:element {
  children (:route)+;

  protected function render(): XHPRoot {
    $matched = null;
    foreach ($this->getChildren() as $route) {
      if ($route instanceof :route) {
        $raw_match = $route->get_match();
        $strlen = strlen($raw_match);
        $match = "";
        for( $i = 0; $i <= $strlen; $i++ ) {
          $char = substr( $raw_match, $i, 1 );
          if ($char === "/") {
            $match .= "\\";
          }
          $match .= $char;
        }
        $match = "/" . $match . "$/";
        if (preg_match($match, requestUri()) === 1) {
          $matched = $route;
        }

        if ($matched !== null) {
          break;
        }
      }
    }
    if ($matched === null) {
      $matched = <not-found message="Page not found." />;
    }
    return $matched;
  }
}
