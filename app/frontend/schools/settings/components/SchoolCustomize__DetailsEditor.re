let str = React.string;
let t = I18n.t(~scope="components.SchoolCustomize__DetailsEditor");
let ts = I18n.ts;
type nonrec action =
  | UpdateName(string)
  | UpdateAbout(string)
  | UpdateSaving(bool);
type nonrec state = {
  name: string,
  about: string,
  saving: bool,
  formDirty: bool,
};
let updateButtonText = saving =>
  if (saving) {
    ts("updating") ++ "...";
  } else {
    ts("update");
  };
module UpdateSchoolQuery = [%graphql
  {js|
  mutation UpdateSchoolMutation($name: String!, $about: String!) {
    updateSchool(about: $about, name: $name) {
      success
    }
  }
|js}
];
let optionAbout = about =>
  if (about == "") {
    None;
  } else {
    Some(about);
  };
let updateSchoolQuery = (state, send, updateDetailsCB) => {
  send(UpdateSaving(true));
  let variables =
    UpdateSchoolQuery.makeVariables(~name=state.name, ~about=state.about, ());
  UpdateSchoolQuery.fetch(variables)
  |> Js.Promise.then_((response: UpdateSchoolQuery.t) => {
       if (response.updateSchool.success) {
         updateDetailsCB(state.name, optionAbout(state.about));
       } else {
         send(UpdateSaving(false));
       };
       Js.Promise.resolve();
     })
  |> ignore;
};
let updateButtonDisabled = state =>
  !state.formDirty || state.saving || state.name |> String.length < 1;
let initialState = (name, about) => {
  name,
  about: about |> OptionUtils.default(""),
  saving: false,
  formDirty: false,
};
let reducer = (state, action) =>
  switch (action) {
  | UpdateName(name) => {...state, name, formDirty: true}
  | UpdateAbout(about) => {...state, about, formDirty: true}
  | UpdateSaving(saving) => {...state, saving}
  };
let handleInputChange = (callback, event) => {
  let value = React.Event.Form.target(event)##value;
  callback(value);
};
[@react.component]
let make = (~name, ~about, ~updateDetailsCB) => {
  let (state, send) = React.useReducer(reducer, initialState(name, about));
  <div className="mx-8 pt-8">
    <h5 className="uppercase text-center border-b border-gray-300 pb-2">
      {t("update_details") |> str}
    </h5>
    <DisablingCover disabled={state.saving}>
      <div className="mt-3">
        <label
          className="inline-block tracking-wide text-xs font-semibold"
          htmlFor="details-editor__name">
          {t("school_name") |> str}
        </label>
        <input
          autoFocus=true
          type_="text"
          maxLength=50
          placeholder={t("school_name_placeholder")}
          className="appearance-none block w-full bg-white text-gray-800 border border-gray-300 rounded py-3 px-4 mt-2 leading-tight focus:outline-none focus:bg-white focus:border-transparent focus:ring-2 focus:ring-focusColor-500"
          id="details-editor__name"
          onChange={handleInputChange(name => send(UpdateName(name)))}
          value={state.name}
        />
        <School__InputGroupError
          message={t("school_name_error")}
          active={state.name |> String.length < 2}
        />
      </div>
      <div className="mt-3">
        <label
          className="inline-block tracking-wide text-xs font-semibold"
          htmlFor="details-editor__about">
          {t("about_label") |> str}
          <span className="font-normal">
            {" " ++ t("max_characters") |> str}
          </span>
        </label>
        <textarea
          maxLength=500
          rows=7
          placeholder={t("details_placeholder")}
          className="appearance-none block w-full bg-white text-gray-800 border border-gray-300 rounded py-3 px-4 mt-2 leading-tight focus:outline-none focus:bg-white focus:border-transparent focus:ring-2 focus:ring-focusColor-500"
          id="details-editor__about"
          onChange={handleInputChange(about => send(UpdateAbout(about)))}
          value={state.about}
        />
      </div>
      <button
        key="details-editor__update-button"
        onClick={_ => updateSchoolQuery(state, send, updateDetailsCB)}
        disabled={updateButtonDisabled(state)}
        className="w-full btn btn-primary btn-large mt-3">
        {updateButtonText(state.saving) |> str}
      </button>
    </DisablingCover>
  </div>;
};
