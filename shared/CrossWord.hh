<?hh
require_once("shared/CrossWordGrid.hh");

class CrossWord {
  public function __construct(private Vector<CrossWordString> $strings, private CrossWordGrid $grid) {

  }

  public function get_grid(): CrossWordGrid {
    return $this->grid;
  }
}
