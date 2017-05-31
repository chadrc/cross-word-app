<?hh
require_once("../shared/page.hh");
require_once("../shared/requests.php");
require_once("../shared/cross-word.hh");

$words = postString("words");
$hints = postString("hints");

$words_array = new Vector(explode(",", $words));
$hints_array = new Vector(explode(",", $hints));

// var_dump(trim($words_array[0]));
// var_dump($hints_array[0]);

if ($words_array->count() !== $hints_array->count()) {
  echo (
    <page title="Couldn't Create Cross Word">
      <h3>Number of words and hints did not match.</h3>
      <a href="create.php">Try Again</a>
    </page>
  );
} else {
  $word_hint_map = Map {};
  for ($i=0; $i<$words_array->count(); $i++) {
    $word = trim($words_array[$i]);
    $hint = trim($hints_array[$i]);
    $word_hint_map[$word] = $hint;
  }

  $cross_word = null;
  try {
    $cross_word = create_cross_word($word_hint_map);
  } catch (CrossWordGenerationException $e) {
    echo (
      <page title="Couldn't Create Cross Word">
        <h3>Sorry, we were unable to generate a cross word with your words.</h3>
        <h5>You may either adjust your words or try to submit again.</h5>
      </page>
    );
  }

  $id = uniqid();
  $json = json_encode(get_object_vars(new CrossWordStore($cross_word)));
  file_put_contents("store/" . $id . ".xw.json", $json);

  if ($cross_word !== null) {
    $horz_words = Vector {};
    $vert_words = Vector {};
    foreach ($cross_word->get_words() as $word) {
      if ($word->is_horizontal()) {
        $horz_words[] = $word;
      } else {
        $vert_words[] = $word;
      }
    }

    $horz_list = <ul></ul>;
    $vert_list = <ul></ul>;

    $num = 1;
    foreach ($horz_words as $word) {
      $horz_list->appendChild(
        <li>{$num}. {$word->get_word()}</li>
      );
      $num += 2;
    }

    $num = 2;
    foreach ($vert_words as $word) {
      $vert_list->appendChild(
        <li>{$num}. {$word->get_word()}</li>
      );
      $num += 2;
    }

    $grid = $cross_word->get_grid();
    $limits = $grid->get_limits();
    $table = <table></table>;
    for ($x=$limits->max_y(); $x>=$limits->min_y(); $x--) {
      $table_row = <tr></tr>;
      for ($y=$limits->min_x(); $y<$limits->max_x(); $y++) {
        $cell = $grid->get_cell($y, $x);
        $table_row->appendChild(
          <td class={$cell === null ? "empty" : ""}></td>
        );
      }
      $table->appendChild($table_row);
    }

    echo (
      <page title="Cross Word Created">
        <section class="cross-word">
          {$table}
        </section>
        <section class="word-hints">
          {$horz_list}
          {$vert_list}
        </section>
      </page>
    );
  }
}

class CrossWordStore {
  public array $words;

  public function __construct(CrossWord $cross_word) {
    $words = $cross_word->get_words();
    $this->words = [];
    foreach ($words as $word) {
      $this->words[] = new CrossWordStringStore($word);
    }
  }
}

class CrossWordStringStore {
  public array $cells;
  public string $word;
  public string $hint;

  public function __construct(CrossWordString $string) {
    $this->word = $string->get_word();
    $this->hint = $string->get_hint();
    $this->cells = [];
    foreach ($string->get_cells() as $cell) {
      $this->cells[] = new CrossWordCellStore($cell);
    }
  }
}

class CrossWordCellStore {
  public string $letter;
  public int $x;
  public int $y;

  public function __construct(CrossWordCell $cell) {
    $this->letter = $cell->get_letter();
    $this->x = $cell->get_x();
    $this->y = $cell->get_y();
  }
}
