<?hh
require_once("../src/cross-word/CrossWordGrid.hh");

class CrossWord {
  private string $id;
  private DateTime $created;

  public function __construct(private Vector<CrossWordString> $strings, private CrossWordGrid $grid) {
    $this->id = "";
    $this->created = new DateTime();
  }

  public function get_grid(): CrossWordGrid {
    return $this->grid;
  }

  public function get_words(): Vector<CrossWordString> {
    return $this->strings;
  }

  public function set_id(string $id) {
    $this->id = $id;
  }

  public function get_id(): string {
    return $this->id;
  }

  public function set_created(DateTime $datetime) {
    $this->created = $datetime;
  }

  public function get_created(): DateTime {
    return $this->created;
  }
}
