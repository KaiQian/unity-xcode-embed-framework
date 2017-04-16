using UnityEngine;
using System.IO;
using System.Collections.Generic;
using UnityEditor;
using UnityEditor.Callbacks;
#if UNITY_IPHONE
using UnityEditor.iOS.Xcode;
#endif

public class XcodeAddEmbedFramework
{
	[PostProcessBuild (1)]
	public static void OnPostprocessBuild (BuildTarget buildTarget, string path)
	{
		if (buildTarget == BuildTarget.iOS) {
			#if UNITY_IPHONE

			string projPath = PBXProject.GetPBXProjectPath (path);
			PBXProject proj = new PBXProject ();
			proj.ReadFromString (File.ReadAllText (projPath));
			string target = proj.TargetGuidByName ("Unity-iPhone");

			// sample of a embed framework and its dependencies 
			proj.AddFrameworkToProject (target, "libz.tbd", false);
			proj.AddFrameworkToProject (target, "libiconv.tbd", false);
			proj.AddFrameworkToProject (target, "libstdc++.tbd", false);
			proj.AddFrameworkToProject (target, "libbz2.tbd", false);
			proj.AddFrameworkToProject (target, "VideoToolbox.framework", false);
			proj.AddFrameworkToProject (target, "UIKit.framework", false);
			proj.AddFrameworkToProject (target, "MediaPlayer.framework", false);
			proj.AddFrameworkToProject (target, "Foundation.framework", false);
			proj.AddFrameworkToProject (target, "CoreGraphics.framework", false);
			proj.AddFrameworkToProject (target, "AudioToolbox.framework", false);
			proj.AddFrameworkToProject (target, "AVFoundation.framework", false);

			// add a folder with all its contents (recursively) as a group of xcode project
			string fileName = Application.dataPath.Replace ("Assets", "AliyunVod");
			CopyAndReplaceDirectory (fileName, Path.Combine (path, "AliyunVod"));
			List<string> filePaths = new List<string> ();
			AddFilesToBuild (ref filePaths, path, "AliyunVod");
			foreach (var filepath in filePaths)
				proj.AddFileToBuild (target, proj.AddFile (filepath, filepath));
			proj.AddBuildProperty (target, "FRAMEWORK_SEARCH_PATHS", "$(PROJECT_DIR)/AliyunVod");

			// some other settings
			proj.SetBuildProperty (target, "ENABLE_BITCODE", "NO");

			// embed framework is supported after iOS 8.0
			proj.SetBuildProperty (target, "IPHONEOS_DEPLOYMENT_TARGET", "8.0");

			proj.WriteToFile (projPath);

			// add a framework as a embed framework in xcode project
			AddFrameworkToEmbed (path, "AliyunVodPlayerSDK.framework");
			#endif
		}
	}

	#if UNITY_IPHONE
	public static void CopyAndReplaceDirectory (string srcPath, string dstPath)
	{
		if (Directory.Exists (dstPath))
			Directory.Delete (dstPath);
		if (File.Exists (dstPath))
			File.Delete (dstPath);

		Directory.CreateDirectory (dstPath);

		foreach (var file in Directory.GetFiles(srcPath))
			File.Copy (file, Path.Combine (dstPath, Path.GetFileName (file)));

		foreach (var dir in Directory.GetDirectories(srcPath))
			CopyAndReplaceDirectory (dir, Path.Combine (dstPath, Path.GetFileName (dir)));
	}

	public static void AddFilesToBuild (ref List<string> filepaths, string projPath, string pathToProj)
	{
		var path = Path.Combine (projPath, pathToProj);
		string[] directories = Directory.GetDirectories (path, "*", SearchOption.TopDirectoryOnly);
		for (int i = 0; i < directories.Length; i++) {
			string directory = directories [i];
			var eachPathToProj = directory.Replace (projPath, "").Substring (1);
			// treat framework & bundle folder as a file
			if (!directory.EndsWith ("framework") && !directory.EndsWith ("bundle")) {
				AddFilesToBuild (ref filepaths, projPath, eachPathToProj);
			} else {
				if (!filepaths.Contains (eachPathToProj))
					filepaths.Add (eachPathToProj);
			}
		}

		string[] files = Directory.GetFiles (path, "*", SearchOption.TopDirectoryOnly);
		for (int i = 0; i < files.Length; i++) {
			string file = files [i];
			string extension = Path.GetExtension (file);
			// ignore system files and unity files
			if (extension != ".DS_Store" && extension != ".meta") {
				var eachPathToProj = file.Replace (projPath, "").Substring (1);
				if (!filepaths.Contains (eachPathToProj))
					filepaths.Add (eachPathToProj);
			}
		}
	}

	public static void AddFrameworkToEmbed (string path, string frameworkName)
	{
		var filePath = Path.Combine (path, "Unity-iPhone.xcodeproj/project.pbxproj");
		int tokenLength = 24;

		var lines = new List<string> (File.ReadAllLines (filePath));

		// add embed framework line
		var index = GetLineIndexThatContains (lines, frameworkName + " in Frameworks");
		var line = lines [index];
		var frameworkUuid = line.Substring (0, line.IndexOf (@"/*")).Trim ();
		var fileRef = line.Substring (line.IndexOf ("fileRef = ") + "fileRef = ".Length, tokenLength);
		var embedFrameworkLineUuid = GetNextUuid (frameworkUuid);
		var embedFrameworkLine = "\t\t" + embedFrameworkLineUuid + " /* " + frameworkName + " in Embed Frameworks */ = {isa = PBXBuildFile; fileRef = " + fileRef + " /* " + frameworkName + " */; settings = {ATTRIBUTES = (CodeSignOnCopy, RemoveHeadersOnCopy, ); }; };";
		lines.Insert (index + 1, embedFrameworkLine);

		// add embed framework section if it does not exist, or insert the line
		index = GetLineIndexThatContains (lines, "/* Begin PBXCopyFilesBuildPhase section */");
		var embedFrameworkSectionLineIndex = GetLineIndexThatContains (lines, "/* Embed Frameworks */ = {", index);
		if (embedFrameworkSectionLineIndex > -1) {
			index = GetLineIndexThatContains (lines, "files = (", embedFrameworkSectionLineIndex);
			lines.Insert (++index, "\t\t\t\t" + embedFrameworkLineUuid + " /* " + frameworkName + " in Embed Frameworks */,");
		} else {
			var embedFrameworkSectionUuid = GetNextUuid (embedFrameworkLineUuid);
			lines.Insert (++index, "\t\t" + embedFrameworkSectionUuid + " /* Embed Frameworks */ = {");
			lines.Insert (++index, "\t\t\tisa = PBXCopyFilesBuildPhase;");
			lines.Insert (++index, "\t\t\tbuildActionMask = 2147483647;");
			lines.Insert (++index, "\t\t\tdstPath = \"\";");
			lines.Insert (++index, "\t\t\tdstSubfolderSpec = 10;");
			lines.Insert (++index, "\t\t\tfiles = (");
			lines.Insert (++index, "\t\t\t\t" + embedFrameworkLineUuid + " /* " + frameworkName + " in Embed Frameworks */,");
			lines.Insert (++index, "\t\t\t);");
			lines.Insert (++index, "\t\t\tname = \"Embed Frameworks\";");
			lines.Insert (++index, "\t\t\trunOnlyForDeploymentPostprocessing = 0;");
			lines.Insert (++index, "\t\t};");

			// add reference of the section
			index = GetLineIndexThatContains (lines, "/* Begin PBXNativeTarget section */");
			var indexBuildPhase = GetLineIndexThatContains (lines, "buildPhases = (", index);
			var indexBuildPhaseEnd = GetLineIndexThatContains (lines, ");", indexBuildPhase);
			lines.Insert (indexBuildPhaseEnd, "\t\t\t\t" + embedFrameworkSectionUuid + " /* Embed Frameworks */,");
		}

		File.WriteAllLines (filePath, lines.ToArray ());

		string projPath = PBXProject.GetPBXProjectPath (path);
		PBXProject proj = new PBXProject ();
		proj.ReadFromString (File.ReadAllText (projPath));
		string target = proj.TargetGuidByName ("Unity-iPhone");
		proj.AddBuildProperty (target, "LD_RUNPATH_SEARCH_PATHS", "$(inherited) @executable_path/Frameworks");
		proj.WriteToFile (projPath);
	}

	static int GetLineIndexThatContains (List<string> lines, string content, int start = 0)
	{
		for (int i = start; i < lines.Count; ++i) {
			if (lines [i].Contains (content)) {
				return i;
			}
		}

		return -1;
	}

	static string GetNextUuid (string token)
	{
		System.Text.StringBuilder sb = new System.Text.StringBuilder ();
		for (int i = 0; i < token.Length; ++i) {
			if (i == 7) {
				char c = token [i];
				if (c == '9') {
					c = 'A';
				} else if (c == 'Z' || c == 'F') {
					c = '0';
				} else {
					c = (char)(c + 1);
				}
				sb.Append (c);
			} else {
				sb.Append (token [i]);
			}
		}
		return sb.ToString ();
	}
	#endif
}
