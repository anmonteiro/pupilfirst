type nonrec id = string;
type nonrec t = {
  id,
  coachId: option(string),
  submissionId: string,
  feedback: string,
};
let id = t => t.id;
let coachId = t => t.coachId;
let submissionId = t => t.submissionId;
let feedback = t => t.feedback;
let decode = json =>
  Json.Decode.{
    id: json |> field("id", string),
    coachId: json |> field("coachId", optional(string)),
    submissionId: json |> field("submissionId", string),
    feedback: json |> field("feedback", string),
  };
