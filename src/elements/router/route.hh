<?hh
require_once("../src/elements/BasePage.hh");

class :route extends :x:element {
  attribute string match @required;
  children (:base-page);

  protected function render(): XHPRoot {
    $root = null;
    $first = $this->getChildren()[0];
    if ($first instanceof XHPRoot) {
      return $first;
    }
    return <html></html>;
  }

  public function get_match(): string {
    return $this->:match;
  }
}
