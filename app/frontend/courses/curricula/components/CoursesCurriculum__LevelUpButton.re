open CoursesCurriculum__Types;
let t = I18n.t(~scope="components.CoursesCurriculum__LevelUpButton");
let str = React.string;
module LevelUpQuery = [%graphql
  {js|
   mutation LevelUpMutation($courseId: ID!) {
    levelUp(courseId: $courseId){
      success
      }
    }
 |js}
];
let handleSubmitButton = saving => {
  let submitButtonText = (title, iconClasses) =>
    <span> <FaIcon classes={iconClasses ++ " mr-2"} /> {title |> str} </span>;

  if (saving) {
    submitButtonText(t("button_text_saving"), "fas fa-spinner fa-spin");
  } else {
    submitButtonText(t("button_text_level_up"), "fas fa-flag");
  };
};
let refreshPage = () => Webapi.Dom.(location |> Location.reload);
let createLevelUpQuery = (course, setSaving, event) => {
  event |> React.Event.Mouse.preventDefault;
  setSaving(_ => true);
  LevelUpQuery.fetch({courseId: Course.id(course)})
  |> Js.Promise.then_((response: LevelUpQuery.t) => {
       if (response.levelUp.success) {
         refreshPage();
       } else {
         setSaving(_ => false);
       };
       Js.Promise.resolve();
     })
  |> ignore;
};
[@react.component]
let make = (~course) => {
  let (saving, setSaving) = React.useState(() => false);
  <button
    disabled=saving
    onClick={createLevelUpQuery(course, setSaving)}
    className="btn btn-success btn-large w-full md:w-2/3 mt-4">
    {handleSubmitButton(saving)}
  </button>;
};
