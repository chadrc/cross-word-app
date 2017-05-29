<?hh
require_once("shared/CrossWordCell.hh");
require_once("shared/utils.hh");

class Coordinate {
  public function __construct(private int $x, private int $y) {}

  public function get_x(): int {
    return $this->x;
  }

  public function get_y(): int {
    return $this->y;
  }
}

class CrossWordGrid implements Printable {
  private Map<int, Map<int, CrossWordCell>> $grid = Map {};

  public function set_cell(int $x, int $y, CrossWordCell $cell) {
    if (!$this->grid->containsKey($y)) {
      $this->grid[$y] = Map {};
    }
    $this->grid[$y][$x] = $cell;
  }

  public function get_cell(int $x, int $y): ?CrossWordCell {
    if ($this->grid->containsKey($y) && $this->grid[$y]->containsKey($x)) {
      return $this->grid[$y][$x];
    }
    return null;
  }

  public function clear_cell(int $x, int $y) {
    if ($this->grid->containsKey($y) && $this->grid[$y]->containsKey($x)) {
      $this->grid[$y]->removeKey($x);
    }
  }

  private function cell_neighbor_positions(CrossWordCell $cell, bool $include_ordinal): Vector<Coordinate> {
    $cx = $cell->get_x();
    $cy = $cell->get_y();

    $positions = Vector {
      new Coordinate($cx, $cy + 1),
      new Coordinate($cx - 1, $cy),
      new Coordinate($cx + 1, $cy),
      new Coordinate($cx, $cy - 1),
    };

    if ($include_ordinal) {
      $positions[] = new Coordinate($cx - 1, $cy + 1);
      $positions[] = new Coordinate($cx + 1, $cy + 1);
      $positions[] = new Coordinate($cx - 1, $cy - 1);
      $positions[] = new Coordinate($cx + 1, $cy - 1);
    }

    return $positions;
  }

  public function cell_neighbors(CrossWordCell $cell, bool $include_ordinal = true): Vector<CrossWordCell> {
    $neighbors = Vector {};
    $neighbors_coordinates = $this->cell_neighbor_positions($cell, $include_ordinal);
    foreach ($neighbors_coordinates as $coord) {
      $neighbor = $this->get_cell($coord->get_x(), $coord->get_y());
      if ($neighbor !== null) {
        $neighbors[] = $neighbor;
      }
    }
    return $neighbors;
  }

  public function cell_neighbor_count(CrossWordCell $cell, bool $include_ordinal = true): int {
    return $this->cell_neighbors($cell, $include_ordinal)->count();
  }

  public function __toString(): string {
    $str = "";
    $lower_x = PHP_INT_MAX;
    $upper_x = PHP_INT_MIN;
    $lower_y = PHP_INT_MAX;
    $upper_y = PHP_INT_MIN;

    foreach ($this->grid as $key => $string) {
      foreach ($string as $key => $letter) {
        if ($letter->get_x() > $upper_x) {
          $upper_x = $letter->get_x();
        }

        if ($letter->get_x() < $lower_x) {
          $lower_x = $letter->get_x();
        }

        if ($letter->get_y() > $upper_y) {
          $upper_y = $letter->get_y();
        }

        if ($letter->get_y() < $lower_y) {
          $lower_y = $letter->get_y();
        }
      }
    }

    for ($row=$upper_y; $row>=$lower_y; $row--) {
      $row_cells = null;
      try {
        $row_cells = $this->grid[$row];
      } catch (Exception $e) {
        continue;
      }
      $str = $str . "| ";
      for ($col=$lower_x; $col<=$upper_x; $col++) {
        $col_cell = null;
        try {
          $col_cell = $row_cells[$col];
        } catch (Exception $e) {
          $str = $str . "  | ";
          continue;
        }
        $str = $str . $col_cell->get_letter() . " | ";
      }
      $str = $str . "\n";
    }
    return $str;
  }
}
