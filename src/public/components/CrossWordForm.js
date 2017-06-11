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
                              onFocus={() => this.wordFocused()}
                              forceFocus={this.state.forceFocusIndex === index}
                              forceFocusName={this.state.forceFocusName}
                              tabIndex={index * 2 + 2} />
          })}
          <WordInput  onFocus={(name) => this.extraWordInputFocused(name)}
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
    if (this.props.forceFocus) {
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
}
