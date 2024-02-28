open StudentsEditor__Types;
let t = I18n.t(~scope="components.StudentCreator__StudentInfoForm");
let ts = I18n.ts;
type nonrec state = {
  name: string,
  email: string,
  title: string,
  affiliation: string,
  hasNameError: bool,
  hasEmailError: bool,
  tagsToApply: array(string),
  teamName: string,
};
type nonrec action =
  | UpdateName(string, bool)
  | UpdateEmail(string, bool)
  | UpdateTitle(string)
  | UpdateTeamName(string)
  | UpdateAffiliation(string)
  | ResetForm
  | AddTag(string)
  | RemoveTag(string);
let str = React.string;
let updateName = (send, name) => {
  let hasError = Js.String.length(name) < 2;
  send(UpdateName(name, hasError));
};
let updateEmail = (send, email) => {
  let regex = [%re {js|/.+@.+\..+/i|js}];
  let hasError = !Js.Re.test(regex, ~str=email);
  send(UpdateEmail(email, hasError));
};
let hasEmailDuplication = (email, emailsToAdd) => {
  let lowerCaseEmail = Js.String.toLowerCase(email);
  Js.Array.some(emailsToAdd, ~f=emailToAdd =>
    lowerCaseEmail == Js.String.toLowerCase(emailToAdd)
  );
};
let formInvalid = (state, emailsToAdd) =>
  state.name == ""
  || state.email == ""
  || state.hasNameError
  || state.hasEmailError
  || hasEmailDuplication(state.email, emailsToAdd);
let handleAdd = (state, send, emailsToAdd, addToListCB) => {
  let trimmedTeamName = Js.String.trim(state.teamName);
  let teamName =
    if (trimmedTeamName == "") {
      None;
    } else {
      Some(trimmedTeamName);
    };
  if (!formInvalid(state, emailsToAdd)) {
    addToListCB(
      StudentInfo.make(
        ~name=state.name,
        ~email=state.email,
        ~title=state.title,
        ~affiliation=state.affiliation,
      ),
      teamName,
      state.tagsToApply,
    );
    send(ResetForm);
  };
};
let initialState = () => {
  name: "",
  email: "",
  title: "",
  affiliation: "",
  hasNameError: false,
  hasEmailError: false,
  tagsToApply: [||],
  teamName: "",
};
let reducer = (state, action) =>
  switch (action) {
  | UpdateName(name, hasNameError) => {...state, name, hasNameError}
  | UpdateEmail(email, hasEmailError) => {...state, email, hasEmailError}
  | UpdateTitle(title) => {...state, title}
  | UpdateTeamName(teamName) => {...state, teamName}
  | UpdateAffiliation(affiliation) => {...state, affiliation}
  | ResetForm => {
      ...state,
      name: "",
      email: "",
      hasNameError: false,
      hasEmailError: false,
    }
  | AddTag(tag) => {
      ...state,
      tagsToApply: Js.Array.concat(state.tagsToApply, ~other=[|tag|]),
    }
  | RemoveTag(tag) => {
      ...state,
      tagsToApply: Js.Array.filter(state.tagsToApply, ~f=t => t != tag),
    }
  };
[@react.component]
let make = (~addToListCB, ~teamTags, ~emailsToAdd, ~disabled) => {
  let (state, send) = React.useReducer(reducer, initialState());
  <div>
    <div className="pt-6">
      <label className="block text-sm font-medium" htmlFor="name">
        {t("name.label")->str}
      </label>
      <input
        value={state.name}
        onChange={event =>
          updateName(send, React.Event.Form.target(event)##value)
        }
        className="appearance-none block w-full bg-white border border-gray-300 rounded py-2.5 px-3 mt-1 text-sm focus:outline-none focus:bg-white focus:border-transparent focus:ring-2 focus:ring-focuscolor-500"
        id="name"
        type_="text"
        placeholder={t("name.placeholder")}
      />
      <School__InputGroupError
        message={t("name.message")}
        active={state.hasNameError}
      />
    </div>
    <div className="pt-6">
      <label className="block text-sm font-medium" htmlFor="email">
        {t("email.label")->str}
      </label>
      <input
        value={state.email}
        onChange={event =>
          updateEmail(send, React.Event.Form.target(event)##value)
        }
        className="appearance-none block w-full bg-white border border-gray-300 rounded py-2.5 px-3 mt-1 text-sm focus:outline-none focus:bg-white focus:border-transparent focus:ring-2 focus:ring-focuscolor-500"
        id="email"
        type_="email"
        placeholder={t("email.placeholder")}
      />
      <School__InputGroupError
        message={
                  if (state.hasEmailError) {
                    t("email.error.invalid");
                  } else {
                    hasEmailDuplication(state.email, emailsToAdd)
                      ? t("email.error.not_unique") : "";
                  }
                }
        active={
          state.hasEmailError || hasEmailDuplication(state.email, emailsToAdd)
        }
      />
    </div>
    <div className="pt-6">
      <label className="block text-sm font-medium" htmlFor="title">
        {t("title.label")->str}
        <span className="inline-block text-xs ml-1">
          {ts("optional_braces")->str}
        </span>
      </label>
      <input
        value={state.title}
        onChange={event =>
          send(UpdateTitle(React.Event.Form.target(event)##value))
        }
        className="appearance-none block w-full bg-white border border-gray-300 rounded py-2.5 px-3 mt-1 text-sm focus:outline-none focus:bg-white focus:border-transparent focus:ring-2 focus:ring-focuscolor-500"
        id="title"
        type_="text"
        placeholder={t("title.placeholder")}
      />
    </div>
    <div className="pt-6">
      <label className="block text-sm font-medium" htmlFor="affiliation">
        {t("affiliation.label")->str}
        <span className="text-xs ml-1"> {ts("optional_braces")->str} </span>
      </label>
      <input
        value={state.affiliation}
        onChange={event =>
          send(UpdateAffiliation(React.Event.Form.target(event)##value))
        }
        className="appearance-none block w-full bg-white border border-gray-300 rounded py-2.5 px-3 mt-1 text-sm focus:outline-none focus:bg-white focus:border-transparent focus:ring-2 focus:ring-focuscolor-500"
        id="affiliation"
        type_="text"
        placeholder={t("affiliation.placeholder")}
      />
    </div>
    <div className="pt-6">
      <div className="flex items-center space-x-2">
        <label className="block text-sm font-medium" htmlFor="team_name">
          {t("team.label")->str}
          <span className="text-xs ml-1"> {ts("optional_braces")->str} </span>
        </label>
        <HelpIcon className="ml-1"> {t("team.help")->str} </HelpIcon>
      </div>
      <input
        value={state.teamName}
        onChange={event =>
          send(UpdateTeamName(React.Event.Form.target(event)##value))
        }
        className="appearance-none block w-full bg-white border border-gray-300 rounded py-2.5 px-3 mt-1 text-sm focus:outline-none focus:bg-white focus:border-transparent focus:ring-2 focus:ring-focusColor-500"
        id="team_name"
        maxLength=50
        type_="text"
        placeholder={t("team.placeholder")}
      />
    </div>
    <div className="pt-6">
      <label className="block text-sm font-medium" htmlFor="tags">
        {t("tags.label")->str}
        <span className="text-xs ml-1"> {ts("optional_braces")->str} </span>
      </label>
    </div>
    <School__SearchableTagList
      unselectedTags={Js.Array.filter(teamTags, ~f=tag =>
        !Js.Array.includes(state.tagsToApply, ~value=tag)
      )}
      selectedTags={state.tagsToApply}
      addTagCB={tag => send(AddTag(tag))}
      removeTagCB={tag => send(RemoveTag(tag))}
      allowNewTags=true
      disabled
    />
    <button
      onClick={_e => handleAdd(state, send, emailsToAdd, addToListCB)}
      disabled={formInvalid(state, emailsToAdd)}
      className={
        "btn btn-primary w-full mt-5"
        ++ (
          if (formInvalid(state, emailsToAdd)) {
            " disabled";
          } else {
            "";
          }
        )
      }>
      <i className="fas fa-plus mr-2" />
      {t("add_to_list")->str}
    </button>
  </div>;
};
