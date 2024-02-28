exception UnknownKindOfLink(string);
type nonrec schoolStrings = {
  address: option(string),
  emailAddress: option(string),
  privacyPolicy: option(string),
  termsAndConditions: option(string),
};
type nonrec file = {
  url: string,
  filename: string,
};
type nonrec schoolImages = {
  logoOnLightBg: option(file),
  coverImage: option(file),
  icon: file,
};
type nonrec linkId = string;
type nonrec title = string;
type nonrec url = string;
type nonrec sortIndex = int;
type nonrec link =
  | HeaderLink(linkId, title, url, sortIndex)
  | FooterLink(linkId, title, url, sortIndex)
  | SocialLink(linkId, url, sortIndex);
type nonrec t = {
  schoolStrings,
  schoolImages,
  links: array(link),
};
let logoOnLightBg = t => t.schoolImages.logoOnLightBg;
let icon = t => t.schoolImages.icon;
let coverImage = t => t.schoolImages.coverImage;
let url = file => file.url;
let filename = file => file.filename;
let address = t => t.schoolStrings.address;
let emailAddress = t => t.schoolStrings.emailAddress;
let privacyPolicy = t => t.schoolStrings.privacyPolicy;
let termsAndConditions = t => t.schoolStrings.termsAndConditions;
let filterLinks = (~header=false, ~footer=false, ~social=false, t) =>
  t.links
  ->(
      Js.Array.filter(~f=l =>
        switch (l) {
        | HeaderLink(_, _, _, _) => header
        | FooterLink(_, _, _, _) => footer
        | SocialLink(_, _, _) => social
        }
      )
    );
let unpackLinks = links =>
  links->(
           Js.Array.map(~f=l =>
             switch (l) {
             | HeaderLink(id, title, url, sortIndex)
             | FooterLink(id, title, url, sortIndex) => (
                 id,
                 title,
                 url,
                 sortIndex,
               )
             | SocialLink(id, url, sortIndex) => (id, "", url, sortIndex)
             }
           )
         );
let addLink = (link, t) => {
  ...t,
  links: t.links->(Js.Array.concat(~other=[|link|])),
};
let removeLink = (linkId, t) => {
  ...t,
  links:
    t.links
    ->(
        Js.Array.filter(~f=l =>
          switch (l) {
          | HeaderLink(id, _, _, _)
          | FooterLink(id, _, _, _) => id != linkId
          | SocialLink(id, _, _) => id != linkId
          }
        )
      ),
};
let updateLink = (linkId, newTitle, newUrl, t) => {
  ...t,
  links:
    t.links
    ->(
        Js.Array.map(~f=l =>
          switch (l) {
          | HeaderLink(id, title, url, sortIndex) =>
            if (id == linkId) {
              HeaderLink(id, newTitle, newUrl, sortIndex);
            } else {
              HeaderLink(id, title, url, sortIndex);
            }
          | FooterLink(id, title, url, sortIndex) =>
            if (id == linkId) {
              FooterLink(id, newTitle, newUrl, sortIndex);
            } else {
              FooterLink(id, title, url, sortIndex);
            }
          | SocialLink(id, url, sortIndex) =>
            if (id == linkId) {
              SocialLink(id, newUrl, sortIndex);
            } else {
              SocialLink(id, url, sortIndex);
            }
          }
        )
      ),
};
type nonrec direction =
  | Up
  | Down;
let sortIndexOfLink = link =>
  switch (link) {
  | HeaderLink(_, _, _, sortIndex)
  | FooterLink(_, _, _, sortIndex)
  | SocialLink(_, _, sortIndex) => sortIndex
  };
let optionalString = s =>
  switch (s->String.trim) {
  | "" => None
  | nonEmptyString => Some(nonEmptyString)
  };
let updatePrivacyPolicy = (privacyPolicy, t) => {
  ...t,
  schoolStrings: {
    ...t.schoolStrings,
    privacyPolicy: privacyPolicy->optionalString,
  },
};
let updateTermsAndConditions = (termsAndConditions, t) => {
  ...t,
  schoolStrings: {
    ...t.schoolStrings,
    termsAndConditions: termsAndConditions->optionalString,
  },
};
let updateAddress = (address, t) => {
  ...t,
  schoolStrings: {
    ...t.schoolStrings,
    address: address->optionalString,
  },
};
let updateEmailAddress = (emailAddress, t) => {
  ...t,
  schoolStrings: {
    ...t.schoolStrings,
    emailAddress: emailAddress->optionalString,
  },
};
let decodeFile = json =>
  Json.Decode.{
    url: json |> field("url", string),
    filename: json |> field("filename", string),
  };
let decodeImages = json =>
  Json.Decode.{
    logoOnLightBg: json |> field("logoOnLightBg", optional(decodeFile)),
    coverImage: json |> field("coverImage", optional(decodeFile)),
    icon: json |> field("icon", decodeFile),
  };
let updateImages = (json, t) => {...t, schoolImages: json |> decodeImages};
let decodeStrings = json =>
  Json.Decode.{
    address: json |> field("address", optional(string)),
    emailAddress: json |> field("emailAddress", optional(string)),
    privacyPolicy: json |> field("privacyPolicy", optional(string)),
    termsAndConditions:
      json |> field("termsAndConditions", optional(string)),
  };
let decodeLink = json => {
  let (kind, id, url, sortIndex) =
    Json.Decode.(
      field("kind", string, json),
      field("id", string, json),
      field("url", string, json),
      field("sortIndex", int, json),
    );
  let title =
    switch (kind) {
    | "header"
    | "footer" => Json.Decode.(field("title", string, json))
    | _ => ""
    };
  switch (kind) {
  | "header" => HeaderLink(id, title, url, sortIndex)
  | "footer" => FooterLink(id, title, url, sortIndex)
  | "social" => SocialLink(id, url, sortIndex)
  | unknownKind => raise(UnknownKindOfLink(unknownKind))
  };
};
let decode = json =>
  Json.Decode.{
    schoolStrings: json |> field("strings", decodeStrings),
    schoolImages: json |> field("images", decodeImages),
    links:
      json
      |> field("links", array(decodeLink))
      |> Js.Array.sortInPlaceWith(~f=(l1, l2) =>
           sortIndexOfLink(l1) - sortIndexOfLink(l2)
         ),
  };
