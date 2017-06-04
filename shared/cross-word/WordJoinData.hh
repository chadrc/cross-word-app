<?hh

class WordJoinData {
  private CrossWordCell $join_letter;
  private Vector<CrossWordCell> $multi_joins;
  private CrossWordString $word;
  private CrossWordCell $existing_letter;

  public function __construct(CrossWordCell $letter, CrossWordString $word, Vector<CrossWordString> $placed) {
    $this->word = $word;
    $this->existing_letter = $letter;
    $options = $word->get_cells_with_letter($letter->get_letter());
    $roll = rand(0, $options->count()-1);
    $this->join_letter = $options[$roll]; // Letter in placing word that will overlap $letter
    // echo "chosen join letter " . $join_letter . "\n";

    $align_vert = $letter->get_owner()->is_horizontal();

    $possible_multi = Vector {};
    foreach ($placed as $placed_word) {
      // Ignore words that are oriented same as new placed word will be
      if ($align_vert && $placed_word->is_verticle()
      || !$align_vert && $placed_word->is_horizontal()) {
        continue;
      }

      foreach ($placed_word->get_cells() as $cell) {
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

    $this->multi_joins = Vector {};
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
      $join_pos = $this->join_letter->get_position();
      // echo $join_pos . "\n";
      $join_index = NAN;
      if ($align_vert) {
        $join_index = $join_pos + $dif;
      } else {
        $join_index = $join_pos - $dif;
      }
      $multi_letter = $this->join_letter->get_owner()->get_cell($join_index);
      if ($multi_letter === null) {
        continue;
      }
      // echo $multi_letter . "\n";

      if ($multi_letter->get_letter() === $pm->get_letter()) {
        $this->multi_joins[] = $pm;
        // echo "is multi join\n";
      }
    }
  }

  public function get_existing_letter(): CrossWordCell {
    return $this->existing_letter;
  }

  public function get_word(): CrossWordString {
    return $this->word;
  }

  public function get_join_letter(): CrossWordCell {
    return $this->join_letter;
  }

  public function get_multi_joins(): Vector<CrossWordCell> {
    return $this->multi_joins;
  }
}
