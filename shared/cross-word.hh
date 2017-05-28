<?hh
require_once("shared/CrossWord.hh");
require_once("shared/utils.hh");

function create_cross_word(Map<string, string> $wordHintMap): CrossWord {
  $words = Vector {};
  foreach($wordHintMap as $key => $value) {
    $words[] = new CrossWordString(strtolower($key), $value);
  }

  // print_words($words);

  $placed = Vector {};
  $grid = new CrossWordGrid();

  if ($words->count() > 0) {
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
      if ($common->count() > 0) {
        shuffle_letters($common);
        $letter = $common[0]; // Letter already in grid to be overlapped with
        // echo "chosen placed letter " . $letter . "\n";
        $options = $value->get_cells_with_letter($letter->get_letter());
        $roll = rand(0, $options->count()-1);
        $join_letter = $options[$roll]; // Letter in placing word that will overlap $letter
        // echo "chosen join letter " . $join_letter . "\n";

        $pos_dif = $join_letter->get_position();
        if ($letter->get_owner()->is_horizontal()) {
          // echo "horizontal\n";
          $start_y = $letter->get_y() + $pos_dif;
          $x = $letter->get_x();

          foreach ($value->get_cells() as $cell) {
            $cell->set_position($x, $start_y);
            $existing = $grid->get_cell($x, $start_y);
            if ($existing !== null) {
              $existing->join($cell);
            } else {
              $grid->set_cell($x, $start_y, $cell);
            }
            $start_y--;
          }
        } else {
          // echo "vertical\n";
          $start_x = $letter->get_x() - $pos_dif;
          $y = $letter->get_y();

          foreach($value->get_cells() as $cell) {
            $cell->set_position($start_x, $y);
            $existing = $grid->get_cell($start_x, $y);
            if ($existing !== null) {
              $existing->join($cell);
            } else {
              $grid->set_cell($start_x, $y, $cell);
            }
            $start_x++;
          }
        }
        $placed[] = $value;
      }
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

function shuffle_letters(Vector<CrossWordCell> $letters) {
  for ($i=0; $i<$letters->count(); $i++) {
    $roll = rand($i, $letters->count()-1);
    $temp = $letters[$i];
    $letters[$i] = $letters[$roll];
    $letters[$roll] = $temp;
  }
}
