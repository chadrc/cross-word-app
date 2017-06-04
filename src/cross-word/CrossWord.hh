<?hh
require_once("../src/cross-word/CrossWordGrid.hh");

class CrossWord {
  public function __construct(private Vector<CrossWordString> $strings, private CrossWordGrid $grid) {

  }

  public function get_grid(): CrossWordGrid {
    return $this->grid;
  }

  public function get_words(): Vector<CrossWordString> {
    return $this->strings;
  }
}