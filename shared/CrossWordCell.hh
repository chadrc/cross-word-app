<?hh
require_once("shared/CrossWordString.hh");
require_once("shared/utils.hh");

class CrossWordCell implements Printable {
  private ?CrossWordCell $top = null;
  private ?CrossWordCell $right = null;
  private ?CrossWordCell $bottom = null;
  private ?CrossWordCell $left = null;

  private ?CrossWordCell $join = null;

  private int $x = -10000;
  private int $y = -10000;

  public function __construct(private CrossWordString $owner, private int $position, private string $letter) {

  }

  public function set_position(int $x, int $y) {
    $this->x = $x;
    $this->y = $y;
  }

  public function get_letter(): string {
    return $this->letter;
  }

  public function get_owner(): CrossWordString {
    return $this->owner;
  }

  public function get_position(): int {
    return $this->position;
  }

  public function get_x(): int {
    return $this->x;
  }

  public function get_y(): int {
    return $this->y;
  }

  public function join(CrossWordCell $cell) {
    $this->join = $cell;
  }

  public function clear_join() {
    $this->join = null;
  }

  public function get_join(): ?CrossWordCell {
    return $this->join;
  }

  public function __toString(): string {
    return $this->letter . " at " . $this->position . " in " . $this->owner->get_word();
  }
}
