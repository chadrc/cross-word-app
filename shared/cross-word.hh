<?hh
require_once("shared/CrossWord.hh");
require_once("shared/utils.hh");

function create_cross_word(Map<string, string> $wordHintMap): CrossWord {
  $attempts = Vector {};
  $cross_word = null;
  while ($attempts->count() < 10) {
    try {
      $cross_word = make_cross_word($wordHintMap);
      break;
    } catch (CrossWordGenerationException $e) {
      $attempts[] = $e;
    }
  }
  if ($cross_word === null) {
    throw new CrossWordCreationFailed($attempts);
  }
  return $cross_word;
}

function make_cross_word(Map<string, string> $wordHintMap): CrossWord {
  $words = Vector {};
  foreach($wordHintMap as $key => $value) {
    $words[] = new CrossWordString(strtolower($key), $value);
  }

  // print_words($words);

  $placed = Vector {};
  $grid = new CrossWordGrid();

  if ($words->count() > 0) {
    shuffle_vec($words);

    $first = $words[0];
    $index = 0;
    foreach ($first->get_cells() as $value) {
      $value->set_position($index, 0);
      $grid->set_cell($index, 0, $value);
      $index++;
    }
    $placed[] = $first;
    $words->removeKey(0);

    // echo "After removal\n";
    // print_words($words);

    foreach ($words as $value) {
      // echo "Checking " . $value->get_word() . " against\n";
      // print_vector($placed);
      $common = get_common_letters($placed, $value);
      // print_vector($common);

      if ($common->count() === 0) {
        throw new NoCommonLetters($value, $placed, $grid);
      }

      shuffle_vec($common);
      $letter = null; // Letter already in grid to be overlapped with
      foreach ($common as $c) {
        $neighbor_count = $grid->cell_neighbor_count($c);
        if ($c->get_join() === null && $neighbor_count <= 2) {
          $letter = $c;
          break;
        }
      }

      if ($letter === null) {
        throw new AllCommonLettersOccluded($value, $placed, $grid);
      }

      // echo "chosen placed letter " . $letter . "\n";
      $options = $value->get_cells_with_letter($letter->get_letter());
      $roll = rand(0, $options->count()-1);
      $join_letter = $options[$roll]; // Letter in placing word that will overlap $letter
      // echo "chosen join letter " . $join_letter . "\n";

      $align_vert = $letter->get_owner()->is_horizontal();

      $possible_multi = Vector {};
      foreach ($placed as $word) {
        // Ignore words that are oriented same as new placed word will be
        if ($align_vert && $word->is_verticle()
        || !$align_vert && $word->is_horizontal()) {
          continue;
        }

        foreach ($word->get_cells() as $cell) {
          if ($cell === $letter) {
            // Already joining on this letter
            continue;
          }

          if ($align_vert) {
            if ($cell->get_x() === $letter->get_x()) {
              $possible_multi[] = $cell;
            }
          } else {
            if ($cell->get_y() === $letter->get_y()) {
              $possible_multi[] = $cell;
            }
          }

        }
      }

      $multi_joins = Vector {};
      foreach ($possible_multi as $pm) {
        // Check if the letters that would be overlapping are the same
        // echo "checking multi " . $pm . "\n";
        $dif = NAN;
        if ($align_vert) {
          $dif = $letter->get_y() - $pm->get_y();
        } else {
          $dif = $letter->get_x() - $pm->get_x();
        }

        // echo $dif . "\n";
        $join_pos = $join_letter->get_position();
        // echo $join_pos . "\n";
        $join_index = NAN;
        if ($align_vert) {
          $join_index = $join_pos + $dif;
        } else {
          $join_index = $join_pos - $dif;
        }
        $multi_letter = $join_letter->get_owner()->get_cell($join_index);
        if ($multi_letter === null) {
          continue;
        }
        // echo $multi_letter . "\n";

        if ($multi_letter->get_letter() === $pm->get_letter()) {
          $multi_joins[] = $pm;
          // echo "is multi join\n";
        }
      }

      // Lambda to capture local state instead of passing it through to another function
      $join_func = ($x, $y, $cell) ==> {
        $cell->set_position($x, $y);
        $existing = $grid->get_cell($x, $y);
        if ($existing !== null) {
          $existing->join($cell);
          $cell->join($existing);
        } else {
          $neighbors = $grid->cell_neighbors($cell);
          foreach ($neighbors as $neighbor) {
            $is_multi_join = false;
            foreach($multi_joins as $mj) {
              if ($neighbor->get_owner() === $mj->get_owner()) {
                $is_multi_join = true;
              }
            }
            if ($neighbor->get_owner() !== $letter->get_owner()
            && $neighbor->get_owner() !== $value
            && !$is_multi_join) {
              echo "cannot place " . ($is_multi_join ? 'true' : 'false') . "\n";
              throw new CannotPlaceWord(Pair{$letter, $join_letter}, $value, $placed, $grid);
            }
          }
          $grid->set_cell($x, $y, $cell);
        }
      };

      $pos_dif = $join_letter->get_position();
      if ($letter->get_owner()->is_horizontal()) {
        // echo "horizontal\n";
        $start_y = $letter->get_y() + $pos_dif;
        $x = $letter->get_x();
        foreach ($value->get_cells() as $cell) {
          $join_func($x, $start_y, $cell);
          $start_y--;
        }
      } else {
        // echo "vertical\n";
        $start_x = $letter->get_x() - $pos_dif;
        $y = $letter->get_y();
        foreach($value->get_cells() as $cell) {
          $join_func($start_x, $y, $cell);
          $start_x++;
        }
      }
      $placed[] = $value;
    }
  }
  return new CrossWord($words, $grid);
}

function get_common_letters(Vector<CrossWordString> $placed, CrossWordString $placing): Vector<CrossWordCell> {
  $common = Vector {};
  foreach ($placed as $word) {
    foreach ($word->get_cells() as $cell) {
      if ($placing->contains_letter($cell->get_letter())) {
        $common[] = $cell;
      }
    }
  }
  return $common;
}

class NoCommonLetters extends CrossWordGenerationException {}
class AllCommonLettersOccluded extends CrossWordGenerationException {}

class CannotPlaceWord extends CrossWordGenerationException {
  public function __construct(
    private Pair<CrossWordCell, CrossWordCell> $join_pair,
    private CrossWordString $placing,
    private Vector<CrossWordString> $placed,
    private CrossWordGrid $grid) {
    parent::__construct($placing, $placed, $grid);
  }

  public function get_join_pair(): Pair<CrossWordCell, CrossWordCell> {
    return $this->join_pair;
  }
}

class CrossWordGenerationException extends Exception {
  public function __construct(
  private CrossWordString $placing,
  private Vector<CrossWordString> $placed,
  private CrossWordGrid $grid) {
    parent::__construct();
  }

  public function get_placing(): CrossWordString {
    return $this->placing;
  }

  public function get_placed(): Vector<CrossWordString> {
    return $this->placed;
  }

  public function get_grid(): CrossWordGrid {
    return $this->grid;
  }
}

class CrossWordCreationFailed extends Exception {
  public function __construct(private Vector<CrossWordGenerationException> $attempts) {
    parent::__construct();
  }

  public function get_attempts(): Vector<CrossWordGenerationException> {
    return $this->attempts;
  }
}
