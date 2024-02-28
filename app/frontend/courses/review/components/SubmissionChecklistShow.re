let str = React.string;
let t = I18n.t(~scope="components.SubmissionChecklistShow");
[@react.component]
let make = (~checklist, ~updateChecklistCB) =>
  <div className="space-y-8">
    {if (ArrayUtils.isEmpty(checklist)) {
       <div> {t("target_marked_as_complete")->str} </div>;
     } else {
       checklist
       ->(
           Js.Array.mapi(~f=(checklistItem, index) =>
             <SubmissionChecklistItemShow
               key={string_of_int(index)}
               index
               checklistItem
               updateChecklistCB
               checklist
             />
           )
         )
       ->React.array;
     }}
  </div>;
