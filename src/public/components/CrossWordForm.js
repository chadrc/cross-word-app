class CrossWordForm extends React.Component {
  constructor(props) {
    super(props);
    this.state = this.defaultState;
  }

  get defaultState() {
    let words = [];
    let title = "";

    if (localStorage.puzzleDraft) {
      let store = JSON.parse(localStorage.puzzleDraft);
      words = store.words;
      title = store.title;
    } else {
      localStorage.puzzleDraft = JSON.stringify({
        title: "",
        words: []
      });
    }

    return {
      title: title,
      words: words,
      forceFocusIndex: null,
      createdPuzzleId: ""
    }
  }

  storeState() {
    localStorage.puzzleDraft = JSON.stringify({
      title: this.state.title,
      words: this.state.words
    });
  }

  addWord() {
    this.state.words.push({
      word: "",
      hint: ""
    });
    let index = this.state.words.length - 1;
    this.setState({words: this.state.words})
    return index;
  }

  extraWordInputFocused(name) {
    let newWordIndex = this.addWord();
    this.setState({
      forceFocusIndex: newWordIndex,
      forceFocusName: name
    });
  }

  wordFocused() {
    this.setState({
      forceFocusIndex: null
    });
  }

  titleChanged(value) {
    this.setState({
      title: value
    }, () => this.storeState());
  }

  wordChanged(index, value) {
    this.state.words[index].word = value;
    this.setState({
      words: this.state.words
    }, () => this.storeState());
  }

  hintChanged(index, value) {
    this.state.words[index].hint = value;
    this.setState({
      words: this.state.words
    }, () => this.storeState());
  }

  submit(e) {
    e.preventDefault();
    e.stopPropagation();
    fetch("/puzzle/make", {
      method: "POST",
      body: JSON.stringify({
        title: this.state.title,
        words: this.state.words,
        redirect: "follow"
      })
    }).then((response) => {
      return response.json()
    }).then((obj) => {
      this.setState({
        createdPuzzleId: obj.puzzleId
      })
    });
  }

  restart() {
    localStorage.puzzleDraft = "";
    this.setState(this.defaultState);
  }

  render() {
    if (this.state.createdPuzzleId) {
      return (
        <section className="puzzle-created-prompt">
          <h2>Puzzle Created</h2>
          <a href={`/puzzle/${this.state.createdPuzzleId}`}>View</a>
          <button type="button" onClick={() => this.restart()}>Make Another</button>
        </section>
      );
    }

    return (
      <form onSubmit={(e) => this.submit(e)}>
        <section>
          <label htmlFor="title">Title</label>
          <input  tabIndex={1}
                  type="text"
                  id="title"
                  value={this.state.title}
                  onChange={(e) => this.titleChanged(e.target.value)} />
        </section>
        <section>
          <label>Words</label>
          {this.state.words.map((item, index) => {
            return <WordInput key={index}
                              word={item.word}
                              hint={item.hint}
                              onFocus={() => this.wordFocused()}
                              onWordChange={(value) => this.wordChanged(index, value)}
                              onHintChange={(value) => this.hintChanged(index, value)}
                              forceFocus={this.state.forceFocusIndex === index}
                              forceFocusName={this.state.forceFocusName}
                              tabIndex={index * 2 + 2} />
          })}
          <WordInput  onFocus={(name) => this.extraWordInputFocused(name)}
                      tabIndex={this.state.words.length * 2 + 2} />
        </section>
        <button type="submit">Create</button>
      </form>
    );
  }
}

window.CrossWordForm = CrossWordForm;

class WordInput extends React.Component {
  constructor(props) {
    super(props);
  }

  componentDidMount() {
    if (this.props.forceFocus) {
      if (this.props.forceFocusName === "word") {
        this.wordInput.focus();
      } else if (this.props.forceFocusName === "hint") {
        this.hintInput.focus();
      }
    }
  }

  raiseWordChanged(value) {
    if (this.props.onWordChange) {
      this.props.onWordChange(value);
    }
  }

  raiseHintChanged(value) {
    if (this.props.onHintChange) {
      this.props.onHintChange(value);
    }
  }

  raiseOnFocused(name) {
    if (this.props.onFocus) {
      this.props.onFocus(name);
    }
  }

  render() {
    return (
      <div className="word-input">
        <input  ref={(input) => this.wordInput = input}
                tabIndex={this.props.tabIndex}
                type="text"
                value={this.props.word}
                onFocus={() => this.raiseOnFocused("word")}
                onChange={(e) => this.raiseWordChanged(e.target.value)} />
        <textarea ref={(input) => this.hintInput = input}
                  tabIndex={this.props.tabIndex + 1}
                  rows={2} type="text"
                  value={this.props.hint}
                  onFocus={() => this.raiseOnFocused("hint")}
                  onChange={(e) => this.raiseHintChanged(e.target.value)} />
      </div>
    );
  }
}
