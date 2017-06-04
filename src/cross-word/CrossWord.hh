<?hh
require_once("../src/cross-word/CrossWordGrid.hh");

class CrossWord {
  private DateTime $created;
  public function __construct(private Vector<CrossWordString> $strings, private CrossWordGrid $grid) {
    $this->created = new DateTime();
  }

  public function get_grid(): CrossWordGrid {
    return $this->grid;
  }

  public function get_words(): Vector<CrossWordString> {
    return $this->strings;
  }

  public function set_created(DateTime $datetime) {
    $this->created = $datetime;
  }

  public function get_created(): DateTime {
    return $this->created;
  }
}
