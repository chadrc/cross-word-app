class CrossWordForm extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      title: "",
      words: [],
      forceFocusIndex: null
    };
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

  extraWordInputFocused() {
    let newWordIndex = this.addWord();
    console.log("new word:", newWordIndex);
    this.setState({
      forceFocusIndex: newWordIndex
    });
  }

  wordFocused(name) {
    console.log("word focus");
    this.setState({
      forceFocusIndex: null,
      forceFocusName: name
    });
  }

  render() {
    return (
      <form>
        <section>
          <label htmlFor="title">Title</label>
          <input tabIndex={1} type="text" id="title" />
        </section>
        <section>
          <label>Words</label>
          {this.state.words.map((item, index) => {
            return <WordInput key={index}
                              onFocus={(name) => this.wordFocused(name)}
                              forceFocus={this.state.forceFocusIndex === index}
                              tabIndex={index * 2 + 2} />
          })}
          <WordInput  onFocus={() => this.extraWordInputFocused()}
                      tabIndex={this.state.words.length * 2 + 2} />
        </section>
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
    console.log("did mount");
    if (this.props.forceFocus) {
      console.log("force focus");
      if (this.props.forceFocusName === "word") {
        this.wordInput.focus();
      } else if (this.props.forceFocusName === "hint") {
        this.hintInput.focus();
      }
    }
  }

  raiseWordChanged(value) {

  }

  raiseHintChanged(value) {

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
                onFocus={() => this.raiseOnFocused("word")}
                onChange={(e) => this.raiseWordChanged(e.target.value)} />
        <textarea ref={(input) => this.hintInput = input}
                  tabIndex={this.props.tabIndex + 1}
                  rows={2} type="text"
                  onFocus={() => this.raiseOnFocused("hint")}
                  onChange={(e) => this.raiseHintChanged(e.target.value)} />
      </div>
    );
  }

  componentDidUpdate() {
    console.log("did update");
    if (this.props.forceFocus) {
      console.log("force focus");
      this.word.focus();
    }
  }
}
