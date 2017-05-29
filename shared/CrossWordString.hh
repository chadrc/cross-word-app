<?hh
require_once("shared/CrossWordCell.hh");
require_once("shared/utils.hh");

class CrossWordString implements Printable {
  private Vector<CrossWordCell> $cells = Vector {};

  public function __construct(private string $word, private string $hint) {
    $strlen = strlen( $word );
    for( $i = 0; $i <= $strlen-1; $i++ ) {
        $char = substr( $word, $i, 1 );
        $this->cells[] = new CrossWordCell($this, $i, $char);
    }
  }

  public function get_cells(): Vector<CrossWordCell> {
    return $this->cells;
  }

  public function get_cell(int $pos): ?CrossWordCell {
    if ($this->cells->containsKey($pos)) {
      return $this->cells[$pos];
    }
    return null;
  }

  public function get_word(): string {
    return $this->word;
  }

  public function contains_letter(string $letter): bool {
    foreach ($this->cells as $cell) {
      if ($cell->get_letter() === $letter) {
        return true;
      }
    }
    return false;
  }

  public function get_cells_with_letter(string $letter): Vector<CrossWordCell> {
    $cells = Vector {};
    foreach ($this->cells as $cell) {
      if ($cell->get_letter() === $letter) {
        $cells[] = $cell;
      }
    }
    return $cells;
  }

  public function is_horizontal(): bool {
    return $this->cells[0]->get_y() === $this->cells[1]->get_y();
  }

  public function is_verticle(): bool {
    return $this->cells[0]->get_x() === $this->cells[1]->get_x();
  }

  public function __toString(): string {
    return $this->word;
  }
}
